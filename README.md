# stapl

> Simple Type-Annotated Programming Language

Inspired by & started as [LLVM Kaleidoscope tutorial](https://llvm.org/docs/tutorial/) implementation [here](https://github.com/sohnryang/playground/tree/main/llvm/kaleidoscope).

[![CMake](https://github.com/sohnryang/stapl/actions/workflows/cmake.yml/badge.svg)](https://github.com/sohnryang/stapl/actions/workflows/cmake.yml)

## How to Build

Prerequisites:

- C++ compiler with C++20 support
- CMake 3.19+
- Boost
- LLVM

```shellsession
$ mkdir build
$ cd build
$ cmake -G 'Ninja' ..
$ ninja
$ ctest
```

The compiled binary is saved to `build/src/staplc`.

## Example: fibonacci sequence

> Note: syntax is subject to change as swipl is in early stage of development.

```
def fib(n: int): int
  if n <= 1 then
    n
  else
    fib(n - 1) + fib(n - 2)

extern put_int(x: int): void
extern scan_int(): int

def main(): void
  put_int(fib(scan_int()))
```

## Roadmap

- [ ] Efficient type checking
- [ ] Support for statements
- [ ] Standard library
- [ ] libc interop
- [ ] More types (arrays, strings, structs, ...)
- [ ] Compiler optimizations
