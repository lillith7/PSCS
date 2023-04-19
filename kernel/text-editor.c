#include "text-editor.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../cpu/timer.h"

// PSCS Built-in text editor.

char *text; // will be malloc'ed later so empty rn

uint8_t exiting = 0; // using uint8_t for a boolean because stdbool takes up 8 bits too
size_t current_size = 64; // size that is malloc'ed for "char *text;"

uint8_t get_exiting() {
    return exiting;
}
size_t get_size_allocated() {
    return current_size;
}

void text_editor_keyboard_callback(registers_t *r) {
    uint8_t scancode = port_byte_in(0x60);
    if (scancode > SC_MAX) return; // unknown key pressed

    if (compare_string(scancode_to_name(scancode), "Esc") == 0) {
        text_editor_exit();
        exiting = 1;
    } else if (scancode == BACKSPACE) {

    } else { // regular key pressed

    }
}

void text_editor_init(void) {
    register_interrupt_handler(IRQ1, text_editor_keyboard_callback);
    clear_screen();
    mem_alloc(current_size);
}

void text_editor_exit(void) {
    register_interrupt_handler(IRQ1, keyboard_callback);
    clear_screen();
    print_string("> ");
}
