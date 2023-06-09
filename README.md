# printf_vector
[<img src="https://img.shields.io/github/license/esrrhs/printf_vector">](https://github.com/esrrhs/printf_vector)
[<img src="https://img.shields.io/github/languages/top/esrrhs/printf_vector">](https://github.com/esrrhs/printf_vector)
[<img src="https://img.shields.io/github/actions/workflow/status/esrrhs/printf_vector/cmake.yml?branch=master">](https://github.com/esrrhs/printf_vector/actions)

like libc printf, but can use vector input.

modify from [eyalroz/printf](https://github.com/eyalroz/printf) to support vector

* for cpp
* header only
* use std::snprintf
* add namespace
* add vector support

## Usage
the example code is in [main.cpp](./main.cpp)
```cpp
printf_vector::vector_input input;
input.add(1);
input.add(2.2f);
input.add("333");
input.add((void *) 0x4);
input.add(2);
input.add("55555");
input.add(5);
input.add(6);
input.add(1.12345);

const char *fmt = "Hello, World! int=%d float=%f string=%s pointer=%p short-string=%.*s width-int=%*d short-float=%.2f\n";
printf_vector::printfv(fmt, &input);
```
and the output is
```bash
Hello, World! int=1 float=2.200000 string=333 pointer=0000000000000004 short-string=55 width-int=    6 short-float=1.12
```
