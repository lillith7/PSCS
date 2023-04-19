#pragma once

// peoples secure computing system built-in program for running arbitrary machine code.
// warning: highly unsafe.

#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../cpu/isr.h"

void machine_code_editor_entry();

void machine_code_editor_exit();

void run_machine_code();