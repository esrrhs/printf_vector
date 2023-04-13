#include <stdio.h>
#include "printf_vector.h"

int main(int argc, char *argv[]) {
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

    return 0;
}
