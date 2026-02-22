#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buff.h"
#include "parser.h"

void help()
{
	fprintf(stderr, "\e[32mUSE ALWAYS: ncc 0.nc 1.nc ... -o out\e[0m\n");
}

int main(int argc, char *argv[])
{
	help();

	system("mkdir -p obj");

	char out[BUFF_LEN] = "";

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			strcpy(out, argv[++i]);	
			break;
		}

		int end = strlen(argv[i]);
		for (int j = end - 3; j < end; j++)
			argv[i][j] = 0;

		parse(argv[i]);
	}
	
	char cmd[BUFF_LEN * 4] = "";
	sprintf(cmd, "gcc obj/*.c -o %s", out);
	system(cmd);

	system("rm -r obj");

	fprintf(stderr, "Done!!! :)\n");
}
