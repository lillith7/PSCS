#include "kernel_panic.h"
#include "../drivers/display.h"

void kernel_panic(char *error_code) {
    clear_screen();
    print_string("KERNEL PANIC! Error code: ");
    print_string(error_code);

    asm volatile("cli");
    asm volatile("hlt");
}