/*
 * NEOC Compiler (aka: NCC), ALPHA 1.1
 * parser.nc
 *
 * It iterates over each char at obj/<FILE *in>, and stores it in a buffer.
 *
 * When it reaches a blanch it decides based on the buffer content what to do:
 * 	if (its a NeoC keyword it parses in a separete function what is suposed)
 * 	to go there; if (not its just outputed as is and cleared (the buffer).)
 *
 * The output is stored at a C or H file.
 * 
 * License: GPL
 * Author:
 * 	trmaa <trmaayt@gmail.com>
 *	25-2-2026
 * 	(Pablo Trik Marín, whatsapp: +34 689 93 75 95)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "buff.h"
static void parse_type( char type[BUFF_LEN])
{	//it happens when let a: int;
	if (type[strlen(type)-1] == ' ')
		type[strlen(type)-1] = 0;

	if (type[strlen(type)-1] == ';')
		type[strlen(type)-1] = 0;
	
	if (type[0] == ' ')
		type++;

	bool pointer = false;
if (type[strlen(type)-1] == '*' ){
		pointer = true;
		type[strlen(type)-1] = 0;
	}

	 char bits =  atoi(&type[1]);

	switch (type[0]) {
	case 'i':
		switch (bits) {
		case 8:
			strcpy(type, "char");
			break;
		case 16:
			strcpy(type, "short");
			break;
		case 32:
			strcpy(type, "int");
			break;
		case 64:
			strcpy(type, "long");
			break;
		}
		break;
	case 'u':
		switch (bits) {
		case 8:
			strcpy(type, "unsigned char");
			break;
		case 16:
			strcpy(type, "unsigned short");
			break;
		case 32:
			strcpy(type, "unsigned int");
			break;
		case 64:
			strcpy(type, "unsigned long");
			break;
		}
		break;
	case 'f':
		switch (bits) {
		case 32:
			strcpy(type, "float");
			break;
		case 64:
			strcpy(type, "double");
			break;
		}
		break;
	}

	if (pointer)
		type[strlen(type)] = '*';
}

static void parse_let( FILE* in,  FILE*  out)
{	char type[BUFF_LEN] = ""; // avoid trash;
char name[BUFF_LEN] = "";
char val[BUFF_LEN] = "";
bool arr = false;
 char c;
	int i = 0;
while ((c = getc(in)) != EOF) {
		if (c == ' ' ){
			goto inferr;
			break;
		}

		else if (c == ':' ){
			goto typed;
			break;
		}

		if (c == '[')
			arr = true;

		name[i++] = c;
	}

	//name was set

inferr:
	fscanf(in, "= %[^\n]\n", val);

	if (val[strlen(val)-1] == ';')
		val[strlen(val)-1] = 0;

	if (val[0] == '\"' ){
		if (arr)
			strcpy(type, "char"); 
		else
			strcpy(type, "char*"); 
	}

	else if (val[0] == '\'')
		strcpy(type, "char");
		//TODO support array -> let arr = { ... }.
		//TODO support references -> let b = &a.
	else if (val[0] == 't' || val[0] == 'f')
		strcpy(type, "bool");
	else
		if (val[strlen(val)-1] == 'f')
			strcpy(type, "float");
		else	
			strcpy(type, "int");

	goto end;
typed:
	{
		int state = 0;
for (i = 0; (c = getc(in)) != EOF;) {
			if (c == '=' && state == 0 ){
				state = 1;
				i = 0;
				continue;
			}

			if (c == '\n')
				break;

			switch (state) {
			case 0:
				type[i++] = c;
				break;
			case 1:
				val[i++] = c;
				break;
			}
		}

		if (type[i-1] == ';')
			type[i-1] = 0;
		if (val[i-1] == ';')
			val[i-1] = 0;
	}
	goto end;
end:
	parse_type(type);

	if (val[0])
		fprintf(out, "%s %s = %s;\n", type, name, val);
	else
		fprintf(out, "%s %s;\n", type, name);
}

static void parse_fn( FILE* in,  FILE*  out)
{	char type[BUFF_LEN] = "";
char whole_func[BUFF_LEN*4] = "";
 char c;
	int i = 0;
while ((c = getc(in)) != EOF && c != '\n') // printf doesn't seem to work in this case...
		whole_func[i++] = c;

	 int len =  i;

	for (int i = (0<len) ? 0 : 0-1;
(0 >= len || i < len) && (0 < len || i >= len);
i += (0<len) ? 1 : -1)
		if (whole_func[i] == '~')
			strcpy(type, &whole_func[i+1]);
			//sprintf(type, "%s", &whole_func[i+1]);
			//sscanf(&whole_func[i], "~%[^.*\n]\n", type);
	
	//type is set
	
	 bool is_defined =  type[strlen(type)-1] != ';';

	if (!is_defined ){
		type[strlen(type)-1] = 0; //eliminate semicotlin
	} else {
		if (type[strlen(type)-1] == '{') {
			type[strlen(type)-1] = 0;
			type[strlen(type)-1] = 0;
		}
	}

	parse_type(type);

	fprintf(out, "%s ", type);

	char name[BUFF_LEN] = "";
for (i = 0; i < len && whole_func[i] != '('; i++)
		name[i] = whole_func[i];

	i++; //now the i is the index of (

	fprintf(out, "%s(", name);

	char var_name[BUFF_LEN] = "";
char var_type[BUFF_LEN] = "";
int fase = 0;
int read = 0;
for (; i < len && whole_func[i] != ')'; i++) {
		c = whole_func[i];

		if (c == ':' ){
			fase = 1;
			read = 0;
			continue;
		} 
		
		if (c == ',' ){
			fase = 0;
			read = 0;

			parse_type(var_type);
			fprintf(out, "%s %s, ", var_type, var_name);
			clear_buff(var_type); //avoid bugs
			clear_buff(var_name);
			continue;
		}

		switch (fase) {
		case 0:
			var_name[read++] = c;
			break;
		case 1:
			var_type[read++] = c;
			break;
		}
	}

	//last param
	parse_type(var_type);

	if (is_defined)
		fprintf(out, "%s %s)\n{", var_type, var_name);
	else
		fprintf(out, "%s %s);\n", var_type, var_name);
}

static void parse_include( FILE* in,  FILE*  out)
{	 char c;

	while ((c = getc(in)) != EOF) {
		putc(c, out);
		if (c == '.')
			break;
	}

	 char _;
	fscanf(in, "n%c%c\n", &c, &_);

	fprintf(out, "%c%c\n", c, _);
}

static void parse_if( FILE* in,  FILE*  out)
{	//Types:
	//if ()
	//if ... {
	//if ... \n
	
	 char c;

	while ((c = getc(in)) != EOF) {
		if (c == '(')
			goto type0;

		if (c != ' ' && c != '(')
			goto type1;
	}

type0:
	putc(c, out);
	while ((c = getc(in)) != EOF && c != '\n')
		putc(c, out);
	putc(c, out);
	return;

type1:
	putc('(', out);
	putc(c, out);
	while ((c = getc(in)) != EOF && c != '\n' && c != '{')
		putc(c, out);
	putc(')', out);
	putc(c, out);
	return;
}

static void parse_for( FILE* in,  FILE*  out)
{	putc('(', out);

	 char c;
	char buff[BUFF_LEN*4] = "";
char name[BUFF_LEN] = "";
char min[BUFF_LEN] = "";
char max[BUFF_LEN] = "";
int i = 0;
while ((c = getc(in)) != EOF && c != '{' && c != '\n') {
		if (c == '(' || c == ')')
			continue;

		buff[i++] = c;
	}

	sscanf(buff, "let %s in %[^.]..%s", name, min, max);

	/*
	 * if (min < Max:)
	 * 	for (int  = (<) ?  : -1;
( >=  ||  < ) && ( <  ||  >= );
 += (<) ? 1 : -1)
	 * else:
	 * 	for (int  = (<) ?  : -1;
( >=  ||  < ) && ( <  ||  >= );
 += (<) ? 1 : -1)
	 */

	//First for statement
	fprintf(out, "int %s = (%s<%s) ? %s : %s-1;\n", name, min, max, min, min);

	//Condition
	/*
	 * We want this
	 *
	 * 	if (min < Max:)
	 * 		i < Max
	 * 	else
	 * 		i >= Max
	 *
	 * Lets write it just with ANDs and ORs as a condition!
	 * 
	 * 		    (m < M <--> i < M)
	 *
	 * 		Simplify to regular arrows.
	 *
	 * 	   (m < M --> i < M) ^ (m >= M --> i >= M)
	 *
	 * 		Transleting arrows to ORs.
	 *
	 *	    (m >= M v i < M) ^ (m < M v i >= M)
	 *
	 * This is the condition that we must check to get that result.
	 * You can test that when m < M, i >= M cancels out, and biceversa.
	 */
	fprintf(out, "(%s >= %s || %s < %s) && (%s < %s || %s >= %s);\n",
			min, max, name, max, min, max, name, max);

	//Increment
	fprintf(out, "%s += (%s<%s) ? 1 : -1", name, min, max);

	putc(')', out);
	putc(c, out);
}

void parse( char name[BUFF_LEN])
{
	 int end =  strlen(name);
	 char ext =  name[end-1];

	for (int i = (end-3<end) ? end-3 : end-3-1;
(end-3 >= end || i < end) && (end-3 < end || i >= end);
i += (end-3<end) ? 1 : -1)
		name[i] = 0;

	 FILE* in;
	 FILE* out;

	{
		char inpath[BUFF_LEN] = "";
char outpath[BUFF_LEN] = "";
sprintf(inpath, "obj/%s.n%c", name, ext);
		sprintf(outpath, "obj/%s.%c", name, ext);

		in = fopen(inpath, "r");
		out = fopen(outpath, "w");
	}

	if (ext == 'h')
		fprintf(out, "#pragma once\n");

	 char c;

	char buff[BUFF_LEN] = ""; // avoid trash;
int buff_index = 0;
bool end_of_keyword = false;
while ((c = getc(in)) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		//parsing
		if (!end_of_keyword ){
			true; //keep on going to avoid bugs with keywords on varnames
		}

		else if (strcmp(buff, "//") == 0 ){
			clear_buff(buff);

			fprintf(out, "//");
			while ((c = getc(in)) != EOF && c != '\n') // ignoring
				putc(c, out);
			putc('\n', out);
		}

		else if (strcmp(buff, "let") == 0 ){
			parse_let(in, out);
		}

		else if (strcmp(buff, "const") == 0 ){
			fprintf(out, "const ");
			parse_let(in, out);
		}

		else if (strcmp(buff, "fn") == 0 ){
			parse_fn(in, out);
		}

		else if (strcmp(buff, "start") == 0 ){
			fprintf(out, "int main(int argc, char *argv[])\n");
		}

		else if (strcmp(buff, "use") == 0 ){
			fprintf(out, "#include ");
			parse_include(in, out);
		}

		else if (strcmp(buff, "if") == 0 ){
			fprintf(out, "if ");
			parse_if(in, out);
		}

		else if (strcmp(buff, "while") == 0 ){
			fprintf(out, "while ");
			parse_if(in, out);
		}

		else if (strcmp(buff, "for") == 0 ){
			fprintf(out, "for ");
			parse_for(in, out);
		}

		else if (strcmp(buff, "cfor") == 0 ){
			fprintf(out, "for ");
		}	

		else {
			fprintf(out, "%s%c", buff, c);
		}

		//reading
		buff[buff_index++] = c;

		//end of a keyword
		if (end_of_keyword ){
			clear_buff(buff);
			buff_index = 0;
		}
	}

	fclose(in);
	fclose(out);
}
