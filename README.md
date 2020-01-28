##	libft Unit tests

> :warning: **Important note: this version targets the new 42 subject of `libft`, if you want to use the old one [you can access it on the old branch](https://github.com/alelievr/libft-unit-test/tree/old).**

libft-unit-test is a complete testing suite for 42's libft project, allowing you to test your lib, track your progress and benchmarking your lib (with system's libc or with another lib)

# Installation
You must have the following file structure:
```
.
|- libft/
|- libft-unit-tests/
```
Whereas:
	`libft` is your project folder.
	`libft-unit-tests` is the folder containing this repository.

Afterwards, you can just `make`.

(NOTE: Before running `make`, you can edit the `LIBFTDIR` variable containing the path to your libft folder.)

# Usage

Run `make f` or `./run_test` when inside the repo's directory. `./run_test -b` to use the Benchmark mode. (`-b` option to compare with system's libc and `-v <libft file>` option to compare with another libft file (should be `libft.so`).

the `-nospeed` option is available if you're mad enough at your optimization. :)

# Linux

To get it work on linux:

+ Install this two packages libbsd-dev and libncurses-dev
+ Add a rule called 'so' in your Makefile to compile your libft in dynamic library instead of static (must be called "libft.so").

# Screenshots

### Default mode
![Unit_Testing](https://user-images.githubusercontent.com/6877923/73291511-95454700-4200-11ea-8a62-bd9f1221e283.png)


### Benchmark mode
![Benchmarking](https://user-images.githubusercontent.com/6877923/73292576-91b2bf80-4202-11ea-8e33-2a1fc22bc2cc.png)

