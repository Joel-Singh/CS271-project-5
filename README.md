# CS271 Project 5 B-trees

## Compiling

There are three make rules for compiling: `main`, `debug`, and `sanitized`.
`main` is the default and compiles normally, `debug` compiles with debug
symbols for use with `gdb` (a debugger) and `sanitized` compiles with
sanitization checks alerting of memory leaks. The binary is always in
`output/main`. E.g to run the program:

```bash
make
./output/main
```
