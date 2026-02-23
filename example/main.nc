use <stdio.nh>
use <stdlib.nh>
use <string.nh>
use "hello.nh"

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
