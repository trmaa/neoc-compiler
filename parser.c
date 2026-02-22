#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "buff.h"

void parse_let()
{
	char type[BUFF_LEN] = ""; // avoid overlaping trash
	char name[BUFF_LEN] = "";
	char val[BUFF_LEN] = "";

	bool arr = false;

	char c;
	char i = 0;

	while ((c = getchar()) != EOF) {
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
	scanf("= %[^;];\n", val);

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
	scanf(" %s = %[^;];\n", type, val);
	goto end;
end:
	if (val[0]) { // got a value
		printf("%s %s = %s;\n", type, name, val);
	} else {
		type[strlen(type)-1] = 0; // eliminating ; bug from inferr
		printf("%s %s;\n", type, name);
	}
}

void parse_fn()
{
	char type[BUFF_LEN] = "";
	char whole_func[BUFF_LEN * 4] = "";

	char c;
	int i = 0;

	while ((c = getchar()) != EOF && c != '{') // printf doesn't seem to work in this case...
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

	printf("%s ", type);

	char name[BUFF_LEN] = "";

	for (i = 0; i < len && whole_func[i] != '('; i++)
		name[i] = whole_func[i];

	i++; // now the i is the index of (

	printf("%s(", name);

	char var_name[BUFF_LEN] = "";
	char var_type[BUFF_LEN] = "";

	char fase = 0;

	char read = 0;

	for (;i < len && whole_func[i] != ')'; i++) {
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

			printf("%s %s, ", var_type, var_name);
			continue;
		}

		switch (fase) {
		case 0:
			var_name[read++] = c;
		case 1:
			var_type[read++] = c;
		}	
	}

	if (is_defined)
		printf("%s %s)\n{", var_type, var_name);
	else
		printf("%s %s);\n", var_type, var_name);
}

void parse()
{
	char c;

	char buff[BUFF_LEN] = ""; // avoid trash
	char buff_index = 0;

	bool end_of_keyword = false;

	while ((c = getchar()) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		// parsing
		if (!strcmp(buff, "//")) {
			while ((c = getchar()) != EOF && c != '\n'); // ignoring
		}

		else if (!strcmp(buff, "let")) {
			parse_let();
		}

		else if (!strcmp(buff, "const")) {
			printf("const ");
			parse_let();
		}

		else if (!strcmp(buff, "fn")) {
			parse_fn();
		}

		else if (!strcmp(buff, "main")) {
			printf("int main(int argc, char *argv[])\n");
		}

		else if (!strcmp(buff, "use")) {
			printf("#include ");
		}

		else {
			if (end_of_keyword)
				printf("%s%c", buff, c);
		}

		// reading
		buff[buff_index++] = c;

		// end of a keyword
		if (end_of_keyword) {
			clear_buff(buff);
			buff_index = 0;
		}
	}
}
