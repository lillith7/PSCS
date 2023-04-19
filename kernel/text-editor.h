#pragma once

#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
// PSCS Built-in text editor.

uint8_t get_exiting();
size_t get_size_allocated();

void text_editor_keyboard_callback(registers_t *r);
void text_editor_init(void);

void text_editor_exit(void);