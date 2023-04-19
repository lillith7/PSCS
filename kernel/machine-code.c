#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../cpu/isr.h"

void machine_code_editor_entry() {
    print_string("Entering Peoples Machine Code Editor, WARNING highly unsafe.\n Press Escape at any time to exit.\n\n> ");

}

void machine_code_editor_exit() {

}

void run_machine_code() {

}