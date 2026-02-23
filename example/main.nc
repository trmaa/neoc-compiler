use <stdio.h>
use <stdlib.h>
use <string.h>
use "hello.h"

start {
	if (argc < 2) {
		printf("Give a name as arg...\n");
		exit(1);
	}

	let name[256]: char;
	strcpy(name, argv[1]);

	let iters = 10;

	for (int i = 0; i < iters; i++) {
		hello(name);
	}
}
