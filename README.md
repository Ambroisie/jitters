# Jitters

This is a small experiment in writing a JIT-compiled calculator. It can
pretty-print, evaluate the AST, compile to assembly, or JIT compile and
interpret the input expression.

## Building

```sh
./bootstrap # Also launches configure
make
```

## How to use

To show the different options, use:

```sh
./jitters --help
```

Once launched, you can write your expression in it's standard input.
