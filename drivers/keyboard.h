#pragma once

#include <stdint.h>
#include "../cpu/isr.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SC_MAX 57


const char* scancode_to_name(uint8_t scancode);
char scancode_to_ascii(uint8_t scancode);
void keyboard_callback(registers_t *regs);
void init_keyboard();
void custom_keyboard_entry(isr_t custom_kb);
void custom_keyboard_exit(void);