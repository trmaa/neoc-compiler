#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "buff.h"

void parse_type(char type[BUFF_LEN])
{
	char bits = atoi(&type[1]);	

	bool pointer = false;

	if (type[strlen(type)-1] == '*') {
		pointer = true;
		type[strlen(type)-1] = 0;
	}

	switch (type[0]) {
	case 'z':
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
	case 'n':
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

void parse_let(FILE *in, FILE *out)
{
	char type[BUFF_LEN] = ""; // avoid overlaping trash
	char name[BUFF_LEN] = "";
	char val[BUFF_LEN] = "";

	bool arr = false;

	char c;
	char i = 0;

	while ((c = getc(in)) != EOF) {
		if (c == ' ') {
			goto inferr;
			break;
		}

		else if (c == ':') {
			goto typed;
			break;
		}

		if (c == '[')
			arr = true;

		name[i++] = c;
	}

	// name was set

inferr:
	fscanf(in, "= %[^;];\n", val);

	if (val[0] == '\"') {
		if (arr)
			strcpy(type, "char"); 
		else
			strcpy(type, "char*"); 
	}

	else if (val[0] == '\'')
		strcpy(type, "char");
		// TODO support array -> let arr = { ... }.
		// TODO support references -> let b = &a.
	else
		if (val[strlen(val)-1] == 'f')
			strcpy(type, "float");
		else	
			strcpy(type, "int");

	goto end;
typed:
	fscanf(in, " %s = %[^;];\n", type, val);
	goto end;
end:
	if (type[strlen(type)-1] == ';') // it happens when let a: int;
		type[strlen(type)-1] = 0;

	parse_type(type);

	if (val[0]) // got a value
		fprintf(out, "%s %s = %s;\n", type, name, val);
	else
		fprintf(out, "%s %s;\n", type, name);
}

void parse_fn(FILE *in, FILE *out)
{
	char type[BUFF_LEN] = "";
	char whole_func[BUFF_LEN * 4] = "";

	char c;
	int i = 0;

	while ((c = getc(in)) != EOF && c != '{') // printf doesn't seem to work in this case...
		whole_func[i++] = c;

	int len = i;

	for (i = 0; i < len; i++)
		if (whole_func[i] == '~')
			sprintf(type, "%s", &whole_func[i+1]);
			//sscanf(&whole_func[i], "~%s", type);
	
	// type is set
	
	type[strlen(type)-1] = 0; // eliminate a space in the end
	
	bool is_defined = type[strlen(type)-1] != ';';

	if (!is_defined)
		type[strlen(type)-1] = 0;

	fprintf(out, "%s ", type);

	char name[BUFF_LEN] = "";

	for (i = 0; i < len && whole_func[i] != '('; i++)
		name[i] = whole_func[i];

	i++; // now the i is the index of (

	fprintf(out, "%s(", name);

	char var_name[BUFF_LEN] = "";
	char var_type[BUFF_LEN] = "";

	char fase = 0;

	char read = 0;

	for (; i < len && whole_func[i] != ')'; i++) {
		c = whole_func[i];

		if (c == ' ')
			continue;

		if (c == ':') {
			fase = 1;
			read = 0;
			continue;
		} 
		
		if (c == ',') {
			fase = 0;
			read = 0;

			parse_type(var_type);

			fprintf(out, "%s %s, ", var_type, var_name);
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

	parse_type(var_type);

	if (is_defined)
		fprintf(out, "%s %s)\n{", var_type, var_name);
	else
		fprintf(out, "%s %s);\n", var_type, var_name);
}

void parse(char *name)
{
	FILE *in, *out;

	{
		char inpath[BUFF_LEN] = "";
		char outpath[BUFF_LEN] = "";

		sprintf(inpath, "%s.nc", name);
		sprintf(outpath, "obj/%s.c", name);

		in = fopen(inpath, "r");
		out = fopen(outpath, "w");
	}

	char c;

	char buff[BUFF_LEN] = ""; // avoid trash
	char buff_index = 0;

	bool end_of_keyword = false;

	while ((c = getc(in)) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		// parsing
		if (!strcmp(buff, "//")) {
			while ((c = getc(in)) != EOF && c != '\n'); // ignoring
		}

		else if (!strcmp(buff, "let")) {
			parse_let(in, out);
		}

		else if (!strcmp(buff, "const")) {
			fprintf(out, "const ");
			parse_let(in, out);
		}

		else if (!strcmp(buff, "fn")) {
			parse_fn(in, out);
		}

		else if (!strcmp(buff, "main")) {
			fprintf(out, "int main(int argc, char *argv[])\n");
		}

		else if (!strcmp(buff, "use")) {
			fprintf(out, "#include ");
		}

		else {
			if (end_of_keyword)
				fprintf(out, "%s%c", buff, c);
		}

		// reading
		buff[buff_index++] = c;

		// end of a keyword
		if (end_of_keyword) {
			clear_buff(buff);
			buff_index = 0;
		}
	}

	fclose(in);
	fclose(out);
}
