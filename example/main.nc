use <stdio.nh>
use <stdlib.nh>
use <string.nh>
use "hello.nh"

start {
	if argc < 2 {
		printf("Give a name as arg...\n");
		exit(1);
	}

	let name[256]: i8;
	strcpy(name, argv[1]);

	const iters = 10;

	for let i in 0..iters {
		if i % 2
			continue;
		hello(name);
	}
}
