// uncomment to enable obnoxious startup chime
// #define STARTUP_CHIME_ENABLED
// define NE2000_ENABLED
#include "defines.h"

#include "../cpu/idt.h"
#include "../cpu/isr.h"

#ifdef RTC_ENABLED
#include "../drivers/rtc.h"
#endif

#ifdef TIMER_ENABLED
#include "../cpu/timer.h"
#endif

#include "../drivers/display.h"
#include "../drivers/keyboard.h"

#include "../drivers/pcspk.h"
#include "../drivers/ata.h"

#ifdef FS_ENABLED
#include "../drivers/ramfs.h"
#endif

#include "util.h"
#include "mem.h"
#include "context-switch.h"

#ifdef BUILT_IN_TEXT_EDITOR_ENABLED
#include "text-editor.h"
#endif

#include "machine-code.h"


#include "shell.h"

void* alloc(int n) {
    int *ptr = (int *) mem_alloc(n * sizeof(int));

    if (ptr == NULL_POINTER) {
        print_string("Memory not allocated.\n");
        return (void*)-1;
    }

    return ptr;
}

uint8_t tmp = 0;
void kernel_sleep() {
    while(1) {
        if(get_switch_task() == 1) {
            set_switch_task(0);
            tmp = !tmp;
            if(tmp) {
                switch_to(5);
            } else {
                switch_to(6);
            }
        }
    }
}

void test_int() {
    print_string("This is a test interrupt; userland interrupts are working!!\n");
    asm volatile("sti");
}

void start_kernel() {
    clear_screen();
    print_string("PSCS STARTING.");
    isr_install();
    print_string("ISR INSTALLED\n");

    asm volatile("sti");

    init_keyboard();
    print_string("KEYBOARD ENABLED ON IRQ 1\n");

    init_dynamic_mem();
    print_string("DYNAMIC MEMORY MANAGER ENABLED\n");

    print_string("init_dynamic_mem()\n");
    print_dynamic_node_size();
    print_dynamic_mem();


#ifdef STARTUP_CHIME_ENABLED 
    play_sound(100);
    time_sleep(200);
    play_sound(1);
    time_sleep(200);
    play_sound(250);
    time_sleep(200);
    nosound();
#endif

#ifdef TIMER_ENABLED
    // init_timer();
    print_string("PIT TIMER ENABLED ON IRQ 0\n");
#endif

#ifdef FS_ENABLED
    init_filesystem();
#endif

    clear_screen();
    print_string("> ");
    rtc_install();
    set_idt_gate(0x40, test_int);
    // asm volatile("int $0x40");
    // create_context(0, kernel_sleep);
    context_switch_test(2);

    #ifdef TIMER_ENABLED
    init_timer();
    // print_string("PIT TIMER ENABLED ON IRQ 0\n");
#endif

    // kernel_sleep();
}
