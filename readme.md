# NEOC

NOTE: This compiler compiles 10 times faster than g++, no kidding...

Is a language which compiles straight to C.

It just has a preatier syntax for those new gen alpha programmers which can't stand `type name = value;`...

Just a fun project for me, i dont intent to make it usefull or nothing.

The program just takes some .nc and .nh files and turns them into a binary by creating
some equivalent .c and .h files and compiling them.

Any C code is a NC valid code too btw, NC is just an extra layer of syntax.

## How to

```console
$ ncc src0.nc src1.nc header0.nh ... -o out
```

## Example

```rust
use <stdio.nh>

fn sum(a: i32, b: i32) ~i32 {
        let sum = 0;

        for let i in a..b
                sum += i; 

        if sum == 0
                printf("WOW!\n");

        return sum;
}

start {
        let a = 10;
        let b = 20;
        let res: i32 = sum(a, b);

        printf("sum(a, b) = %d\n", res);
}
```

## TODO

0. Documentation.
1. Give more type infering support.
2. Error checking.
3. Comments.
4. Function type for args ( void (*func)() ).

## BUGS

+ let arr[len * 4] = ""; inferrs to int because of the number, not char
+ if strlen(str) == n //... doesn't put the last )

## GUIDE

NeoC is a thin layer of syntax ontop of C which makes things just a bit easier.
It provides type inference, foreaches, some typedefs...

So, the language is fully compatible with C, it's libs, and the stdlib...

BTW: almost all C code is valid NeoC code.

### Features

```go

for let i in var0..var1 {
    ...
}

// or

for (let i in var0..var1) {
    ...
}

// or

cfor (int i = var0; i < var1; i++) {
    ...
}

// equals

for (int i = var0; i < var1; i++) {
    ...
}

```

Also, if statements can be writen without parenthesis.

Variables which are initialized can get an infered type as long as they are not
arrays, or structs. Strings, ints, floats, chars, and bools will get inferred.

### Std gideline

Tabs over spaces, and tabs are 8 spaces wide. Rows of text should not excede 80
chars.

Do not cut strings in half.

```go
// wrong
#include <stdio.h>
#define N 10

// right
use <stdio.nh>
def N 10
```

```go
// wrong
char c = 'a';
let c: char = 'a';
let c: char;

// right
let c = 'a';
char c;
```

```go
// wrong
int sum(int a, int b);
// right
fn sum(a: int, b: int) ~int;
```

```go
// wrong
struct color {
    let a: char;
    let b: char;
    let g: char;
    let r: char;
};
// right
struct color { char a, b, g, r; };
```

```go
// wrong
fn main(argc: int, argv[]: char) ~int {}
int main() {}

// right
start {}
fn main() ~int {}
int main(void) {}
```

```go
// this may be better than
fn main() ~int
{
        for let i in 0..10 {
                return 0;
        }
}

//this
fn main() ~int {
        for let i in 0..10 {
                return 0;
        }
}
´´´
