# NEOC

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

fn sum(a: int, b: int) ~int {
    return a + b;
}

start {
    let a = 10;
    let b: z32 = 20;
    let res: int = sum(a, b);

    printf("%d\n", res);
}
```

## TODO

1. Give more type infering support.
3. Error checking.
4. Comments.
