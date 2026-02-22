#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BUFF_LEN 256

void clear_buff(char buff[BUFF_LEN])
{
	for (int i = 0; i < BUFF_LEN; i++)
		buff[i] = 0;
}

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
		// TODO support array { ... } inferrence.
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

int main()
{
	char c;

	char buff[BUFF_LEN] = ""; // avoid trash
	char buff_index = 0;

	bool end_of_keyword = false;

	while ((c = getchar()) != EOF) {
		end_of_keyword = c == ' ' || c == '\n' || c == '\t';

		// parsing
		if (!strcmp(buff, "let")) {
			parse_let();
		}

		else if (!strcmp(buff, "const")) {
			printf("const ");
			parse_let();
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
