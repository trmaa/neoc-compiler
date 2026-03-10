/*
 * NEOC Compiler (aka: NCC), ALPHA 1.1
 * header.nc
 *
 * It creates .nh files resolvig defines, includes, and public declarations.
 *
 * It iterates over the .nc input to search for keywords like pub, def, or use,
 * and runs the apropiate functions to output to the obj/<src>.nc or .nh as
 * needed.
 * 
 * License: GPL
 * Author:
 * 	trmaa <trmaayt@gmail.com>
 *	25-2-2026
 * 	(Pablo Trik Marín, whatsapp: +34 689 93 75 95)
 */

use <stdio>
use <string>
use "buff"

fn parse_use(in: FILE*, out: FILE*) ~static void {
	let c: char;
	let buff[BUFF_LEN] = "";
	let buff_index = 0;

	while ((c = getc(in)) != EOF && c != '\n')
		buff[buff_index++] = c;

	if buff[strlen(buff)-4] == '.' {
		fprintf(out, "%s\n", buff);
		return;
	}

	let end: int = strlen(buff)-1;
	let ext[3] = ".nh";

	let closing: char = buff[end];

	for let i in 0..3
		buff[end+i] = ext[i];

	buff[strlen(buff)] = closing;

	fprintf(out, "%s\n", buff);
}

fn parse_define(in: FILE*, out: FILE*) ~static void {
	let c: char;
	let buff[BUFF_LEN] = "";
	let buff_index = 0;

	while ((c = getc(in)) != EOF && c != '\n')
		buff[buff_index++] = c;
	
	fprintf(out, "%s\n", buff);
}

fn parse_pub(in: FILE*, outh: FILE*, outc: FILE*) ~static void {
	let c: char;
	let buff[BUFF_LEN] = "";
	let buff_index = 0;

	while ((c = getc(in)) != EOF && c != '{')
		buff[buff_index++] = c;
	
	fprintf(outh, "%s;\n", buff);
	fprintf(outc, "%s {\n", buff);
}

pub fn create_headers(name[BUFF_LEN]: char) ~void {
	let end: int = strlen(name);

	for let i in end-3..end
		name[i] = 0;

	let in: FILE*;
	let outc: FILE*;
	let outh: FILE*;

	{
		let inpath[BUFF_LEN] = "";
		let outpathc[BUFF_LEN] = "";
		let outpathh[BUFF_LEN] = "";

		sprintf(inpath, "%s.nc", name);
		sprintf(outpathc, "obj/%s.nc", name);
		sprintf(outpathh, "obj/%s.nh", name);


		in = fopen(inpath, "r");
		outc = fopen(outpathc, "w");
		outh = fopen(outpathh, "w");
	}

	let c: char;
	let buff[BUFF_LEN] = "";
	let buff_index = 0;

	let end_of_keyword = false;

	while ((c = getc(in)) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		if !end_of_keyword {
			buff[buff_index++] = c;
		}

		else if strcmp(buff, "use") == 0 {
			clear_buff(buff);
			buff_index = 0;
			fprintf(outc, "use ");
			parse_use(in, outc);	
		}

		else if strcmp(buff, "def") == 0 {
			clear_buff(buff);
			buff_index = 0;
			fprintf(outh, "#define ");
			parse_define(in, outh);
		}

		else if strcmp(buff, "typedef") == 0 {
			clear_buff(buff);
			buff_index = 0;
			fprintf(outh, "typedef ");
			parse_define(in, outh);
		}

		else if strcmp(buff, "pub") == 0 {
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
