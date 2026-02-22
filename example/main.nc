use <stdio.h>
use <string.h>

fn hello(name: char*) ~void {
	printf("Hello, %s\n", name);
}

main {
	let name[256]: char;
	strcpy(name, argv[1]);

	let iters = 10;

	for (int i = 0; i < iters; i++) {
		hello(name);
	}
}
