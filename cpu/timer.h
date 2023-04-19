#pragma once

#include "../kernel/util.h"

uint32_t get_timer_ms();
uint32_t get_timer_secs();
uint32_t get_timer_mins();
uint32_t get_timer_hours();
uint8_t get_switch_task();
void set_switch_task(uint8_t should);
void timer_reset();
void init_timer();
void init_pit();
void time_sleep(uint32_t sleep_ms);