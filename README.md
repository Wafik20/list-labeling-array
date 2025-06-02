# C Project with Auto-Building Makefile

This is a simple C project that uses a Makefile to automatically rebuild when source files change.

## Building the Project

To build the project, simply run:
```bash
make
```

This will compile all `.c` files in the directory and create an executable named `program`.

## Running the Program

After building, you can run the program with:
```bash
./program
```

## Cleaning Build Files

To clean up all build artifacts, run:
```bash
make clean
```

## Features

- Automatically detects changes in source files
- Compiles with warning flags enabled (-Wall -Wextra)
- Includes debug symbols (-g)
- Simple and clean build process
