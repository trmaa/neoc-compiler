use <stdio.nh>
use <stdlib.nh>
use <string.nh>
use <stdbool.nh>
use "buff.nh"

fn parse_type(type[BUFF_LEN]: char) ~static void {
	// it happens when let a: int;
	if type[strlen(type)-1] == ' '
		type[strlen(type)-1] = 0;

	if type[strlen(type)-1] == ';'
		type[strlen(type)-1] = 0;
	
	if type[0] == ' '
		type++;

	let pointer = false;

	if type[strlen(type)-1] == '*' {
		pointer = true;
		type[strlen(type)-1] = 0;
	}

	let bits: char = atoi(&type[1]);

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

	if pointer
		type[strlen(type)] = '*';
}

fn parse_let(in: FILE*, out: FILE*) ~static void {
	let type[BUFF_LEN] = ""; // avoid trash
	let name[BUFF_LEN] = "";
	let val[BUFF_LEN] = "";

	let arr = false;

	let c: char;
	let i = 0;

	while ((c = getc(in)) != EOF) {
		if c == ' ' {
			goto inferr;
			break;
		}

		else if c == ':' {
			goto typed;
			break;
		}

		if c == '['
			arr = true;

		name[i++] = c;
	}

	// name was set

inferr:
	fscanf(in, "= %[^\n]\n", val);

	if val[strlen(val)-1] == ';'
		val[strlen(val)-1] = 0;

	if val[0] == '\"' {
		if arr
			strcpy(type, "char"); 
		else
			strcpy(type, "char*"); 
	}

	else if val[0] == '\''
		strcpy(type, "char");
		// TODO support array -> let arr = { ... }.
		// TODO support references -> let b = &a.
	else if val[0] == 't' || val[0] == 'f'
		strcpy(type, "bool");
	else
		if val[strlen(val)-1] == 'f'
			strcpy(type, "float");
		else	
			strcpy(type, "int");

	goto end;
typed:
	{
		let state = 0;
		cfor (i = 0; (c = getc(in)) != EOF;) {
			if c == '=' && state == 0 {
				state = 1;
				i = 0;
				continue;
			}

			if c == '\n'
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

		if type[i-1] == ';'
			type[i-1] = 0;
		if val[i-1] == ';'
			val[i-1] = 0;
	}
	goto end;
end:
	parse_type(type);

	if val[0]
		fprintf(out, "%s %s = %s;\n", type, name, val);
	else
		fprintf(out, "%s %s;\n", type, name);
}

fn parse_fn(in: FILE*, out: FILE*) ~static void {
	let type[BUFF_LEN] = "";
	let whole_func[BUFF_LEN*4] = "";

	let c: char;
	let i = 0;

	while ((c = getc(in)) != EOF && c != '\n') // printf doesn't seem to work in this case...
		whole_func[i++] = c;

	let len: int = i;

	for let i in 0..len
		if whole_func[i] == '~'
			strcpy(type, &whole_func[i+1]);
			//sprintf(type, "%s", &whole_func[i+1]);
			//sscanf(&whole_func[i], "~%[^.*\n]\n", type);
	
	// type is set
	
	let is_defined: bool = type[strlen(type)-1] != ';';

	if !is_defined {
		type[strlen(type)-1] = 0; // eliminate semicotlin
	} else {
		if (type[strlen(type)-1] == '{') {
			type[strlen(type)-1] = 0;
			type[strlen(type)-1] = 0;
		}
	}

	parse_type(type);

	fprintf(out, "%s ", type);

	let name[BUFF_LEN] = "";

	cfor (i = 0; i < len && whole_func[i] != '('; i++)
		name[i] = whole_func[i];

	i++; // now the i is the index of (

	fprintf(out, "%s(", name);

	let var_name[BUFF_LEN] = "";
	let var_type[BUFF_LEN] = "";

	let fase = 0;

	let read = 0;

	cfor (; i < len && whole_func[i] != ')'; i++) {
		c = whole_func[i];

		if c == ':' {
			fase = 1;
			read = 0;
			continue;
		} 
		
		if c == ',' {
			fase = 0;
			read = 0;

			parse_type(var_type);
			fprintf(out, "%s %s, ", var_type, var_name);
			clear_buff(var_type); // avoid bugs
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

	// last param
	parse_type(var_type);

	if is_defined
		fprintf(out, "%s %s)\n{", var_type, var_name);
	else
		fprintf(out, "%s %s);\n", var_type, var_name);
}

fn parse_include(in: FILE*, out: FILE*) ~static void {
	let c: char;

	while ((c = getc(in)) != EOF) {
		putc(c, out);
		if c == '.'
			break;
	}

	let _: char;
	fscanf(in, "n%c%c\n", &c, &_);

	fprintf(out, "%c%c\n", c, _);
}

fn parse_if(in: FILE*, out: FILE*) ~static void {
	// Types:
	// if ()
	// if ... {
	// if ... \n
	
	let c: char;

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

fn parse_for(in: FILE*, out: FILE*) ~static void {
	putc('(', out);

	let c: char;
	let buff[BUFF_LEN*4] = "";
	let name[BUFF_LEN] = "";
	let min[BUFF_LEN] = "";
	let max[BUFF_LEN] = "";
	let i = 0;

	while ((c = getc(in)) != EOF && c != '{' && c != '\n') {
		if c == '(' || c == ')'
			continue;

		buff[i++] = c;
	}

	sscanf(buff, "let %s in %[^.]..%s", name, min, max);

	fprintf(out, "int %s = %s; %s < %s; %s++", name, min, name, max, name);

	putc(')', out);
	putc(c, out);
}

fn parse(name: char*) ~void {
	let end: int = strlen(name);
	let ext: char = name[end-1];

	for let i in end-3..end
		name[i] = 0;

	let in: FILE*;
	let out: FILE*;

	{
		let inpath[BUFF_LEN] = "";
		let outpath[BUFF_LEN] = "";

		sprintf(inpath, "%s.n%c", name, ext);
		sprintf(outpath, "obj/%s.%c", name, ext);

		in = fopen(inpath, "r");
		out = fopen(outpath, "w");
	}

	if ext == 'h'
		fprintf(out, "#pragma once\n");

	let c: char;

	let buff[BUFF_LEN] = ""; // avoid trash
	let buff_index = 0;

	let end_of_keyword = false;

	while ((c = getc(in)) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		// parsing
		if !end_of_keyword {
			true; // keep on going to avoid bugs with keywords on varnames
		}

		else if !strcmp(buff, "//") {
			clear_buff(buff);

			fprintf(out, "//");
			while ((c = getc(in)) != EOF && c != '\n') // ignoring
				putc(c, out);
			putc('\n', out);
		}

		else if !strcmp(buff, "let") {
			parse_let(in, out);
		}

		else if !strcmp(buff, "const") {
			fprintf(out, "const ");
			parse_let(in, out);
		}

		else if !strcmp(buff, "fn") {
			parse_fn(in, out);
		}

		else if !strcmp(buff, "start") {
			fprintf(out, "int main(int argc, char *argv[])\n");
		}

		else if !strcmp(buff, "use") {
			fprintf(out, "#include ");
			parse_include(in, out);
		}

		else if !strcmp(buff, "if") {
			fprintf(out, "if ");
			parse_if(in, out);
		}

		else if !strcmp(buff, "while") {
			fprintf(out, "while ");
			parse_if(in, out);
		}

		else if !strcmp(buff, "for") {
			fprintf(out, "for ");
			parse_for(in, out);
		}

		else if !strcmp(buff, "cfor") {
			fprintf(out, "for ");
		}

		else if !strcmp(buff, "def") {
			fprintf(out, "#define ");
		}

		else {
			fprintf(out, "%s%c", buff, c);
		}

		// reading
		buff[buff_index++] = c;

		// end of a keyword
		if end_of_keyword {
			clear_buff(buff);
			buff_index = 0;
		}
	}

	fclose(in);
	fclose(out);
}
