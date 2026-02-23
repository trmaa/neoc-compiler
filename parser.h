#pragma once

#include <stdio.h>

char *parse_type(char *type);
void parse_let(FILE *in, FILE *out);
void parse_fn(FILE *in, FILE *out);
void parse(char *name);
