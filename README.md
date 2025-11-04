# CS271 Project 5 B-trees

## Compiling

There are three make rules for compiling: `main`, `debug`, `sanitized`, and `extended_tests_only`.
`main` is the default and compiles normally, `debug` compiles with debug
symbols for use with `gdb` (a debugger), `sanitized` compiles with
sanitization checks alerting of memory leaks, and `extended_tests_only` only run the extended tests. The binary is always in
`output/main`. E.g to run the program:

```bash
make
./output/main
```

Or to run only the extended tests:

```bash
make extended_tests_only
./output/main
```
