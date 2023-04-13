#include <stdio.h>
#include "printf_vector.h"

int main(int argc, char *argv[]) {
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
    return 0;
}
