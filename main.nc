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
 * Think of it like a giant macro.
 *
 * If there is not -c flag, it removes the obj/ dir.
 *
 * Quite cool, huh??
 * 
 * License: GPL
 * Author: trmaa <trmaayt@gmail.com> 21-2-2026 (Pablo Trik Marín)
 */

use <stdio.nh>
use <stdlib.nh>
use <string.nh>
use <stdbool.nh>
use <time.nh>
use "buff.nh"
use "parser.nh"

fn help() ~void {
	fprintf(stderr, "\e[32mUSE ALWAYS: ncc 0.nc 1.nc ... -o out\e[0m\n");
}

start {
	let begin: clock_t = clock();
	
	let clean = true;
	system("mkdir -p obj");
	let out[BUFF_LEN] = "a.out";
	let flags[BUFF_LEN*4] = "";

	for let i in 1..argc {
		if !strcmp(argv[i], "-o") {
			strcpy(out, argv[++i]);
			continue;
		}

		if !strcmp(argv[i], "-c") {
			clean = false;
			continue;
		}

		if !strcmp(argv[i], "-h") {
			help();
			exit(0); // TODO: clean obj/
		}

		if !strcmp(argv[i], "-f") {
			strcpy(flags, argv[++i]);
			continue;
		}

		printf("\e[1;32mNCC %s\n\e[0m", argv[i]);
		parse(argv[i]);
	}
	
	let cmd[BUFF_LEN*4] = "";
	sprintf(cmd, "gcc obj/*.c -o %s %s", out, flags);
	system(cmd);

	if clean
		system("rm -r obj");
	
	let end: clock_t = clock();
	let elapsed: double = end - begin;
	printf("Done in %d ms!!!\n", elapsed);
}
