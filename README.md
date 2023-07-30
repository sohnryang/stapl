# stapl

> Simple Type-Annotated Programming Language

Inspired by & started as [LLVM Kaleidoscope tutorial](https://llvm.org/docs/tutorial/) implementation [here](https://github.com/sohnryang/playground/tree/main/llvm/kaleidoscope).

[![CMake](https://github.com/sohnryang/stapl/actions/workflows/cmake.yml/badge.svg)](https://github.com/sohnryang/stapl/actions/workflows/cmake.yml)

## How to Build

Prerequisites:

- C++ compiler with C++20 support
- CMake 3.19+
- Boost
- LLVM 16+

```shellsession
$ mkdir build
$ cd build
$ cmake -G 'Ninja' ..
$ ninja
$ ctest
```

The compiled binary is saved to `build/bin/staplc`.

## Example: fibonacci sequence

> Note: syntax is subject to change as stapl is in early stage of development.

```
def fib(n: int): int {
  if n <= 1 {
    return n
  } else {
    return fib(n - 1) + fib(n - 2)
  }
}
```

## Roadmap

- [x] Efficient type checking
- [x] Support for statements: #1
- [ ] Standard library
- [ ] libc interop
- [ ] More types (arrays, strings, structs, ...)
- [ ] Compiler optimizations
