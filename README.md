# printf_vector
Tiny, fast(ish), self-contained and fully loaded printf, sprinf etc. implementation.

modify from [eyalroz/printf](https://github.com/eyalroz/printf) to support vector

* for cpp
* header only
* change some macro to const
* add namespace
* add vector support

## Usage
the example code is in [main.cpp](./main.cpp)
```cpp
#include "printf_vector.h"
printf_vector::va_list_gadget_impl_t input;
input.add(1);
input.add("2");
input.add(3.2f);
input.add((void *) 0xDEADBEEF);
printf_vector::printfv("Hello, World! %d %s %f %p\n", &input);

input.reset_index();

char buffer[100];
printf_vector::snprintfv(buffer, 100, "Hello, World! %d %s %f %p\n", &input);
printf("%s", buffer);
```
