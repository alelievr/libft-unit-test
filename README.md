# libft-unit-test

## INSTALLATION

git clone your_libft_repository libft/

git clone this repository libft-unit-test

cd libft-unit-test

make f or make && ./run_test

type ./run_test -h for help

you can change te default libft path in the makefile (LIBFTDIR variable)


## THE TESTS

This project use the dynamic version of your libft to extract function so:
  * if you haven't finished your libft the test will just display a [MISSING] to the function test.
  * the test will check some bonus function if you code them (ft_strlcpy, ft_atof, ft_strndup, ft_strtrimc, ...)

### Default mode:
  run with ./run_test
  this mode will test the integrity of your functions
  ![alt tag](http://image.noelshack.com/fichiers/2016/35/1472923740-capture-d-ecran-2016-09-03-a-19-26-32.png)

### Bench mode:
  run with ./run_test -b OR ./run_test -v vs_libft.so
  this mode will compare the speed of your libft's first part functions with the system's libc (-b) or with another libft (shared library in parameter with -v)
  ![alt tag](http://image.noelshack.com/fichiers/2016/35/1472923749-capture-d-ecran-2016-09-03-a-19-27-09.png)
  
### Colors:
  red: slow, blue: fast

## CONTRIBUTE

if you want to add new tests/functions to this project, just contact me (@alelievr on slack)
