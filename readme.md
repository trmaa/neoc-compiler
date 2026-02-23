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
