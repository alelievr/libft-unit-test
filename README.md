##	libft Unit tests

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
![Unit Testing](https://camo.githubusercontent.com/d48094cc4a8bccb4610a3990794916ee74be9614/687474703a2f2f696d6167652e6e6f656c736861636b2e636f6d2f66696368696572732f323031362f33352f313437323932333734302d636170747572652d642d656372616e2d323031362d30392d30332d612d31392d32362d33322e706e67)

### Benchmark mode
![Benchmarking](https://camo.githubusercontent.com/89f54b0ab69793e585c503ac2d35ac73f09d053a/687474703a2f2f696d6167652e6e6f656c736861636b2e636f6d2f66696368696572732f323031362f33352f313437323932333734392d636170747572652d642d656372616e2d323031362d30392d30332d612d31392d32372d30392e706e67)
