# NEOC

Is a language which compiles straight to C.

It just has a preatier syntax for those new gen alpha programmers which can't stand `type name = value;`...

Just a fun project for me, i dont intent to make it usefull or nothing.

The program just takes some .nc files and turns them into a binary by creating
some equivalent .c files and compiling them. This means there are no header
files...

Any C code is a NC valid code too btw, NC is just an extra layer of syntax.

## How to

```console
$ ncc src0.nc src1.nc ... -o out
```

You can only use functions which are declared in each file: There are no hedader
files, so just put things in order. You are the compiler now...

## Example

```rust
fn sum(a: int, b: int) int {
    return a + b;
}

fn sum_inf(a: int, b: int) {
    return a + b;
}

main {
    const msg0: char* = "0";
    let msg1: char* = "1";
    const msg2 = "2";
    let msg3 = "3";

    printf("%s\n", msg0);
    printf("%s\n", msg1);
    printf("%s\n", msg2);
    printf("%s\n", msg3);
}
```

Whould be:

```c++
int sum(int a, int b)
{
    return a + b;
}

int sum_inf(int a, int b)
{
    return a + b;
}

int main()
{
    const char *msg0 = "0";
    char *msg1 = "1";
    const char *msg2 = "2";
    char *msg3 = "3";

    printf("%s\n", msg0);
    printf("%s\n", msg1);
    printf("%s\n", msg2);
    printf("%s\n", msg3);
}
```

## TODO

1. Give support to unsigned types.
2. Document the code.
3. Add flags and opts to pause at different stages of compilation.
4. Give more type infering support.
5. in-range for loops.
