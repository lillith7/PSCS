#include "context-switch.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../cpu/isr.h"
#include "../kernel/mem.h"
// software context switching
// written by me :)

context_t contexts[MAX_TASKS];
uint32_t current_pid = 0;
context_t get_context(uint16_t i) {
    return contexts[i];
}
void lock_my_task() {
    contexts[current_pid].locked = 1;
}
void unlock_my_task() {
    contexts[current_pid].locked = 0;
}
uint32_t get_current_pid() {
    return current_pid;
}

void create_context(uint32_t pid, void *function) {
    void *context_stack = mem_alloc(1024 * 64); // 64K stack
    contexts[pid].used = 1;
    contexts[pid].esp = context_stack + 1024;
    contexts[pid].eip = function;
    contexts[pid].locked = 0;
    asm volatile("mov %ebx, %esp");
    asm volatile("mov %%esp, %0" : : "g"(contexts[pid].esp));
    // asm volatile("pushf");
    // asm volatile("pusha");
    asm volatile("mov %0, %%esp" : "=g"(contexts[current_pid].esp));
    asm volatile("mov %esp, %ebx");
}
// void switchtsk(uint32_t *old_esp, uint32_t new_esp) {
//         asm volatile(
//             "push %ebx;"
//             "push %ebp;"
//             "push %esi;"
//             "push %edi;"
//             "mov %esp, (%eax);"
//             "mov %edx, %esp;"
//             "pop %edi;"
//             "pop %esi;"
//             "pop %ebx;"
//             "ret"
//         );
// }

void switch_to(uint32_t pid) {
    if(contexts[current_pid].locked == 1) {
        return;
    } else {
        // asm volatile("mov %0, %%esp" : "=g"(contexts[current_pid].esp));

        // // asm volatile("pushf");
        // asm volatile("mov %%esp, %0" : : "g"(contexts[pid].esp));

        // asm volatile("mov %0, %%eax" : "=g"(contexts[current_pid].eax));
        // asm volatile("mov %%eax, %0" : : "g"(contexts[pid].eax));

        // asm volatile("mov %0, %%ebx" : "=g"(contexts[current_pid].ebx));
        // asm volatile("mov %%ebx, %0" : : "g"(contexts[pid].ebx));

        // asm volatile("mov %0, %%ecx" : "=g"(contexts[current_pid].ecx));
        // asm volatile("mov %%ecx, %0" : : "g"(contexts[pid].ecx));

        // asm volatile("mov %0, %%edx" : "=g"(contexts[current_pid].edx));
        // asm volatile("mov %%edx, %0" : : "g"(contexts[pid].edx));

        // asm volatile("mov %0, %%edi" : "=g"(contexts[current_pid].edi));
        // asm volatile("mov %%edi, %0" : : "g"(contexts[pid].edi));

        // asm volatile("mov %0, %%esi" : "=g"(contexts[current_pid].esi));
        // asm volatile("mov %%esi, %0" : : "g"(contexts[pid].esi));

        // // asm volatile("popf");

        // current_pid = pid;

        // void *address = contexts[pid].eip;
        // goto *address;

    }
}

void test_task() {
        lock_my_task();
        print_string("1");
        unlock_my_task();
        // switch_to(0);
    while(1) {
    }
    return;
}
void test_task_b() {
        lock_my_task();
        print_string("0");
        unlock_my_task();
        // switch_to(0);
    while(1) {
    }
    return;
}
void context_switch_test(uint32_t pid) {
    create_context(5, test_task);
    create_context(6, test_task_b);
    create_context(4, context_switch_test);
    // switch_to(6);
    // switch_to(0);

}
