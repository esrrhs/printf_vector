# printf_vector
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

const char *fmt = "Hello, World! int=%d float=%f string=%s pointer=%p short-string=%.*s width-int=%*d\n";
printf_vector::printfv(fmt, &input);

input.reset_index();

char buffer[1024];
printf_vector::snprintfv(buffer, sizeof(buffer), fmt, &input);
printf("%s", buffer);
```
and the output is
```bash
Hello, World! int=1 float=2.200000 string=333 pointer=0000000000000004 short-string=55 width-int=    6
Hello, World! int=1 float=2.200000 string=333 pointer=0000000000000004 short-string=55 width-int=    6
```