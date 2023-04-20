#pragma once

#include "../kernel/util.h"
#include "../cpu/isr.h"

#include "context-switch.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../cpu/isr.h"
#include "../kernel/mem.h"
#define MAX_TASKS 32
// software context switching
// written by me :)

// typedef struct { (in here for refrence)
//     uint32_t ds; /* Data segment selector */
//     uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
//     uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
//     uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
// } registers_t;

typedef struct {
    uint32_t ds; /* Data segment selector */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
    uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
    int (*pid)();
    uint8_t locked;
    uint8_t used;
} context_t;
typedef uint32_t pid_t;
typedef enum state { running,
                     ready,
                     blocked,
                     Nil } state_t;
typedef struct registers_task {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
    uint16_t es, gs, fs, cs;
} __attribute__((packed)) registers_task_t;

typedef struct task_control_block {
    registers_task_t                regs;
    state_t                    state_task;
    pid_t                      pid;
    struct task_control_block* new_tasks; // field that can be used for multiple different linked lists of tasks later on

} __attribute__((packed)) task_control_block_t;

typedef struct sheduler {
    uint8_t               init_timer;
    uint32_t              task_timer;
    task_control_block_t* running_task;
} sheduler_t;
// software context switching
// written by me :)

// typedef struct {
//     uint32_t ds; /* Data segment selector */
//     uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
//     uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
//     uint32_t pid;
// } context_t;
void lock_my_task();
void unlock_my_task();
uint32_t get_num_tasks();
registers_task_t *get_task(uint32_t i);
uint32_t get_current_pid();
void create_context(uint32_t pid, void *function);
void __switch();
uint8_t get_task_locked(uint32_t pid);
void create_task(task_control_block_t* task, void (*task_func)(), uint32_t eflags, uint32_t cr3);
__attribute__((regparm(3))) void switchtsk( uint32_t *old_esp, uint32_t new_esp );

extern void switch_to_task(registers_task_t*, registers_task_t*);
void switch_to(uint32_t pid);
void test_task();
void context_switch_test(uint32_t pid);
task_control_block_t* get_tcb(uint32_t i);