#pragma once

#include <stdio.h>
#include <stdlib.h>

inline void fatal(char *msg, void (*cb)(int), int status)
{
	fprintf(stderr, "\e[1;31mfatal: %s\n\e[0m", msg);
	cb(status);

	exit(1);
}
