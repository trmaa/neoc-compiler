# NEOC

Is a language which compiles straight to C. It just has a preatier syntax for
those new gen alpha programmers which can't stand `type name = value;`...

Just a fun project for me, i dont intent to make it usefull or nothing.

The program reads from stdin, and prints to stdout, so to compile a file just:
`ncc <in.nc >out.c`.

## Example

```rust
fn sum(a: int, b: int) int {
    return a + b;
}

fn sum_inf(a: int, b: int) {
    return a + b;
}

main()
{
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
