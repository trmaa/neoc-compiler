/*
 * NEOC Compiler (aka: NCC), ALPHA 1.1
 * main.nc
 *
 * USE: ncc [-c don't clean] [-h help] files.nc [-o out path] [-f compile flags]
 *
 * It gets as args the options and the nc files.
 *
 * It loops over the args to solve the options and create "parseable" .nc and .nh
 * files at obj/. Think of this step as the -E step of the gcc compiler. It
 * cleans the code, creating the header files, the includes, and the defines
 * propperly.
 *
 * Then it parses the new .nc and .nh files into actual C code, to then compile
 * that with GCC.
 * 
 * Think of it like a giant C macro.
 *
 * Quite cool, huh??
 * 
 * License: GPL
 * Author:
 * 	trmaa <mail@pablotrik.online>
 *	25-2-2026
 * 	(Pablo Trik Marín, whatsapp: +34 689 93 75 95)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include "buff.h"
#include "parser.h"
#include "header.h"
void help( )
{	fprintf(stderr, "\e[32mUSE ALWAYS: ncc 0.nc 1.nc ... -o out\e[0m\n");
}

int main(int argc, char *argv[])
{
	 clock_t begin =  clock();
	
	bool clean = true;
system("mkdir -p obj");
	system("rm obj/*");
	char out[BUFF_LEN] = "a.out";
char flags[BUFF_LEN*4] = "";
for (int i = (1<argc) ? 1 : 1-1;
(1 >= argc || i < argc) && (1 < argc || i >= argc);
i += (1<argc) ? 1 : -1){
		if (strcmp(argv[i], "-o") == 0 ){
			strcpy(out, argv[++i]);
			continue;
		}

		if (strcmp(argv[i], "-c") == 0 ){
			clean = false;
			continue;
		}

		if (strcmp(argv[i], "-h") == 0 ){
			help();
			exit(0); //TODO: clean obj/
		}

		if (strcmp(argv[i], "-f") == 0 ){
			strcpy(flags, argv[++i]);
			continue;
		}

		create_headers(argv[i]); //pre processes nc code to make nh code into obj/
	}

	DIR *dir;
	struct dirent *entry;
	char path[BUFF_LEN] = "";

	if ((dir = opendir("obj")) == NULL) {
		printf("WTF\n");
		exit(1);
	}

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		strcpy(path, entry->d_name);

		printf("\e[1;32mNCC %s\n\e[0m", path);
		parse(path); //actually compiles the prepocessed code
	}

	closedir(dir);
	
	char cmd[BUFF_LEN*4] = "";
sprintf(cmd, "gcc obj/*.c -o %s %s", out, flags);
	system(cmd);

	if (clean)
		system("rm -r obj");
	
	 clock_t end =  clock();
	 double elapsed =  end - begin;
	printf("Done in %d ms!!!\n", elapsed);
}
