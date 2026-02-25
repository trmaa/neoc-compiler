/*
 * NEOC Compiler (aka: NCC), ALPHA 1.1
 * header.nc
 *
 * It creates .nh files resolvig defines, includes, and public declarations.
 *
 * It iterates over the .nc input to search for (int  = ;  < ; ++)
 * and runs the apropiate functions to output to the obj/<src>.nc or .nh as
 * needed.
 * 
 * License: GPL
 * Author:
 * 	trmaa <trmaayt@gmail.com>
 *	25-2-2026
 * 	(Pablo Trik Marín, whatsapp: +34 689 93 75 95)
 */

#include <stdio.h>
#include <string.h>
#include "buff.h"
static void parse_use( FILE* in,  FILE*  out)
{	 char c;
	char buff[BUFF_LEN] = "";
int buff_index = 0;
while ((c = getc(in)) != EOF && c != '\n')
		buff[buff_index++] = c;

	if (buff[strlen(buff)-4] == '.' ){
		fprintf(out, "%s\n", buff);
		return;
	}

	 int end =  strlen(buff)-1;
	char ext[3] = ".nh";
 char closing =  buff[end];

	for (int i = 0; i < 3; i++)
		buff[end+i] = ext[i];

	buff[strlen(buff)] = closing;

	fprintf(out, "%s\n", buff);
}

static void parse_define( FILE* in,  FILE*  out)
{	 char c;
	char buff[BUFF_LEN] = "";
int buff_index = 0;
while ((c = getc(in)) != EOF && c != '\n')
		buff[buff_index++] = c;
	
	fprintf(out, "%s\n", buff);
}

static void parse_pub( FILE* in,  FILE*  outh,  FILE*  outc)
{	 char c;
	char buff[BUFF_LEN] = "";
int buff_index = 0;
while ((c = getc(in)) != EOF && c != '{')
		buff[buff_index++] = c;
	
	fprintf(outh, "%s;\n", buff);
	fprintf(outc, "%s {\n", buff);
}

void create_headers( char name[BUFF_LEN])
{
	 int end =  strlen(name);

	for (int i = end-3; i < end; i++)
		name[i] = 0;

	 FILE* in;
	 FILE* outc;
	 FILE* outh;

	{
		char inpath[BUFF_LEN] = "";
char outpathc[BUFF_LEN] = "";
char outpathh[BUFF_LEN] = "";
sprintf(inpath, "%s.nc", name);
		sprintf(outpathc, "obj/%s.nc", name);
		sprintf(outpathh, "obj/%s.nh", name);


		in = fopen(inpath, "r");
		outc = fopen(outpathc, "w");
		outh = fopen(outpathh, "w");
	}

	 char c;
	char buff[BUFF_LEN] = "";
int buff_index = 0;
bool end_of_keyword = false;
while ((c = getc(in)) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		if (!end_of_keyword ){
			buff[buff_index++] = c;
		}

		else if (strcmp(buff, "use") == 0 ){
			clear_buff(buff);
			buff_index = 0;
			fprintf(outc, "use ");
			parse_use(in, outc);	
		}

		else if (strcmp(buff, "def") == 0 ){
			clear_buff(buff);
			buff_index = 0;
			fprintf(outh, "#define ");
			parse_define(in, outh);
		}

		else if (strcmp(buff, "pub") == 0 ){
			clear_buff(buff);
			buff_index = 0;
			parse_pub(in, outh, outc);
		}

		else {
			fprintf(outc, "%s%c", buff, c);
			clear_buff(buff);
			buff_index = 0;
		}
	}

	fclose(in);
	fclose(outc);
	fclose(outh);
}
