#include "context-switch.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../cpu/isr.h"
#include "../kernel/mem.h"
// software context switching
// written by me :)

task_control_block_t contexts[MAX_TASKS];
uint8_t task_status[MAX_TASKS];
uint32_t current_pid = 0;
uint32_t num_tasks = 0;

registers_task_t* get_task(uint32_t i) {
    // registers_task_t *tempRegs = &contexts[i].regs;
    return &contexts[i].regs;
}
task_control_block_t* get_tcb(uint32_t i) {
    return &contexts[i];
}
void lock_my_task() {
    task_status[current_pid] = 1;
}
void unlock_my_task() {
    task_status[current_pid] = 0;
}
uint8_t get_task_locked(uint32_t pid) {
    return task_status[pid];
}
uint32_t get_num_tasks() {
    return num_tasks;
}
uint32_t get_current_pid() {
    return current_pid;
}

void create_context(uint32_t pid, void *function) {
    // void *context_stack = mem_alloc(1024 * 64); // 64K stack
    // contexts[pid].used = 1;
    // contexts[pid].esp = context_stack + 1024;
    // contexts[pid].eip = function;
    // contexts[pid].locked = 0;
    // asm volatile("mov %ebx, %esp");
    // asm volatile("mov %%esp, %0" : : "g"(contexts[pid].esp));
    // // asm volatile("pushf");
    // // asm volatile("pusha");
    // asm volatile("mov %0, %%esp" : "=g"(contexts[current_pid].esp));
    // asm volatile("mov %esp, %ebx");
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
    // if(contexts[current_pid].locked == 1) {
    //     return;
    // } else {
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

    // }
}


sheduler_t sheduler;

// static task_control_block_t main_task, task1, task2, task3;

void __switch()
{
    task_control_block_t* prev_task;

    prev_task = sheduler.running_task;

    sheduler.running_task = sheduler.running_task->new_tasks;

    switch_to_task(&(prev_task->regs), &(sheduler.running_task->regs));
}

void init_multitasking()
{
    sheduler.init_timer = 0;
    sheduler.task_timer = 20;
}

void create_task(task_control_block_t* task, void (*task_func)(), uint32_t eflags, uint32_t cr3)
{
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;

    task->regs.eflags = eflags;
    task->regs.eip    = (uint32_t)task_func;
    task->regs.cr3    = (uint32_t)cr3;
    task->regs.esp    = (uint32_t)mem_alloc(1024);
    task->new_tasks   = 0;
    num_tasks++;
    asm ("sti");
}

void test_task() {
    start_a:
    lock_my_task();
    print_string("a");
    unlock_my_task();
    switch_to_task(&(contexts[0].regs), &(contexts[3].regs));
    goto start_a;

    return;
}
void test_task_b() {
    start_b:
    lock_my_task();
    print_string("b");
    unlock_my_task();
    switch_to_task(&(contexts[1].regs), &(contexts[3].regs));
    goto start_b;

    return;
}
void donetsk() {
    while(1);
}
void context_switch_test(uint32_t pid) {
    // create_context(5, test_task);
    // create_context(6, test_task_b);
    // create_context(4, context_switch_test);
    // switch_to(6);
    // switch_to(0);
    init_multitasking();
    create_task(&contexts[0], test_task, 0, 0);
    create_task(&contexts[1], test_task_b, 0, 0);
    create_task(&contexts[2], donetsk, 0, 0);
    // __switch();
    // switch_to_task(&(contexts[2].regs), &(contexts[0].regs));
}
