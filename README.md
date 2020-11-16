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

# Results

The test result will be displayed directly in the console with a simple color code.
Color | Meaning
--- | ---
![](https://via.placeholder.com/15/f00/000000?text=+) Red | The function did not behaved correctly or crashed during the test.
![](https://via.placeholder.com/15/f90/000000?text=+) Orange | The function exceeded the maximal execution time, Probably due to an infinite loop.
![](https://via.placeholder.com/15/ff0/000000?text=+) Yellow | The function failed to handle an extreme case, Most of the time, it's okay to consider the function as correct.
![](https://via.placeholder.com/15/0f0/000000?text=+) Green | The function is correct.

More information about the test context and code is available in the result.log written at the root of the test repository.

Note that in the results, there is a big difference between `[CRASH]` or `[FAIL]` and `[KO]`. While both crash and fail means that the function does not work and should be graded as invalid it's not the case for KO, KO means that the functions fails to handle a case not intended to be tested by 42's grading system and that in some case may exceed the scope of the function.

# Linux

To get it work on linux:

+ Install this two packages libbsd-dev and libncurses-dev
+ Add a rule called 'so' in your Makefile to compile your libft in dynamic library instead of static (must be called "libft.so").


Example:
```
so:
	$(CC) -fPIC $(CFLAGS) $(SRC)
	gcc -shared -o libft.so $(OBJ)
```


# Screenshots

### Default mode
![Unit_Testing](https://user-images.githubusercontent.com/6877923/73291511-95454700-4200-11ea-8a62-bd9f1221e283.png)


### Benchmark mode
![Benchmarking](https://user-images.githubusercontent.com/6877923/73292576-91b2bf80-4202-11ea-8e33-2a1fc22bc2cc.png)

