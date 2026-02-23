/*
 * NEOC Compiler (aka: NCC)
 *
 * USE: ncc [-c don't clean] files.nc [-o out path]
 *
 * It inputs from FILE *in (.nc file), to the parser, and this outputs to 
 * FILE *out (obj/.c file), for each file.
 *
 * Then it compiles with gcc all the generated C files into "out".
 *
 * If there is not -c flag, it removes the obj/ dir.
 *
 * Quite cool, huh??
 * 
 * License: GPL
 * Author: trmaa <trmaayt@gmail.com> 21-2-2026 (Pablo Trik Marín)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "buff.h"
#include "parser.h"

void help()
{
	fprintf(stderr, "\e[32mUSE ALWAYS: ncc 0.nc 1.nc ... -o out\e[0m\n");
}

int main(int argc, char *argv[])
{
	bool clean = true;

	system("mkdir -p obj");

	char out[BUFF_LEN] = "a.out";

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			strcpy(out, argv[++i]);	
			break;
		}

		if (!strcmp(argv[i], "-c")) {
			clean = false;
			continue;
		}

		int end = strlen(argv[i]);
		for (int j = end - 3; j < end; j++)
			argv[i][j] = 0;

		parse(argv[i]);
	}
	
	char cmd[BUFF_LEN * 4] = "";
	sprintf(cmd, "gcc obj/*.c -o %s", out);
	system(cmd);

	if (clean)
		system("rm -r obj");
}
