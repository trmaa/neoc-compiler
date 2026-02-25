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
 * 	trmaa <trmaayt@gmail.com>
 *	25-2-2026
 * 	(Pablo Trik Marín, whatsapp: +34 689 93 75 95)
 */

use <stdio.nh>
use <stdlib.nh>
use <string.nh>
use <stdbool.nh>
use <time.nh>
use <dirent.nh>
use "buff.nh"
use "parser.nh"
use "header.nh"

fn help() ~void {
	fprintf(stderr, "\e[32mUSE ALWAYS: ncc 0.nc 1.nc ... -o out\e[0m\n");
}

start {
	let begin: clock_t = clock();
	
	let clean = true;
	system("mkdir -p obj");
	system("rm obj/*");
	let out[BUFF_LEN] = "a.out";
	let flags[BUFF_LEN*4] = "";

	for let i in 1..argc {
		if strcmp(argv[i], "-o") == 0 {
			strcpy(out, argv[++i]);
			continue;
		}

		if strcmp(argv[i], "-c") == 0 {
			clean = false;
			continue;
		}

		if strcmp(argv[i], "-h") == 0 {
			help();
			exit(0); // TODO: clean obj/
		}

		if strcmp(argv[i], "-f") == 0 {
			strcpy(flags, argv[++i]);
			continue;
		}

		create_headers(argv[i]); // pre processes nc code to make nh code into obj/
	}

	DIR *dir;
	struct dirent *entry;
	char path[BUFF_LEN] = "";

	if ((dir = opendir("obj")) == NULL) {
		printf("WTF\n");
		exit(1);
	}

	while ((entry = readdir(dir)) != NULL) {
		if strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0
			continue;

		strcpy(path, entry->d_name);

		printf("\e[1;32mNCC %s\n\e[0m", path);
		parse(path); // actually compiles the prepocessed code
	}

	closedir(dir);
	
	let cmd[BUFF_LEN*4] = "";
	sprintf(cmd, "gcc obj/*.c -o %s %s", out, flags);
	system(cmd);

	if clean
		system("rm -r obj");
	
	let end: clock_t = clock();
	let elapsed: double = end - begin;
	printf("Done in %d ms!!!\n", elapsed);
}
