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
context_t get_context(uint16_t i);
uint32_t get_current_pid();
void create_context(uint32_t pid, void *function);
__attribute__((regparm(3))) void switchtsk( uint32_t *old_esp, uint32_t new_esp );
void switch_to(uint32_t pid);
void test_task();
void context_switch_test(uint32_t pid);