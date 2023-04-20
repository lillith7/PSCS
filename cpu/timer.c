#include "timer.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/context-switch.h"
#include "isr.h"

enum TIMERS{
    TIMER_PIT,
    TIMER_RTC
};

#define ACTIVE_TIMER TIMER_PIT

uint32_t milliseconds = 0;
uint32_t seconds = 0;
uint32_t minutes = 0;
uint32_t hours = 0;
uint32_t sleep_ms = 0;
uint32_t sleep_ms_goal = 0;
uint8_t sleeping = 0;
uint8_t PIT_ENABLED = 0;
uint8_t should_switch_task = 0;

#define TIMER_CHANNEL_0 0x40
#define TIMER_CHANNEL_1 0x41
#define TIMER_CHANNEL_2 0x42
#define TIMER_COMMAND_PORT 0x43

uint32_t get_timer_ms() {
    return milliseconds;
}
uint32_t get_timer_secs() {
    return seconds;
}
uint32_t get_timer_mins() {
    return minutes;
}
uint32_t get_timer_hours() {
    return hours;
}
uint8_t get_switch_task() {
    return should_switch_task;
}
void set_switch_task(uint8_t should) {
    should_switch_task = should;
}
uint8_t tmpa = 0;
static void pit_timer_callback(registers_t *regs) {
    milliseconds++;

    if (milliseconds % 2) {
        should_switch_task = 1;
    }

    if (milliseconds == 1000) {
        milliseconds = 0;
        seconds++;
        if (seconds == 60){
            seconds = 0;
            minutes++;
            if (minutes == 60) {
                minutes = 0;
                hours++;
            }
        }
    }

    if(should_switch_task == 1) {
        should_switch_task = 0;
        // asm("cli");
        tmpa = !tmpa;
        if(tmpa) {
            switch_to_task(get_task(3), get_task(1));
        } else {
            switch_to_task(get_task(3), get_task(0));
        }
        // asm("sti");
    }
    
    if (sleeping == 1) {
        sleep_ms++;
        if (sleep_ms == sleep_ms_goal) {
            sleeping = 0;
        }
    }
}
static void trc_timer_callback(registers_t *regs) {

}

void timer_reset() {
    milliseconds = 0;
    seconds = 0;
    minutes = 0;
    hours = 0;
}
void init_timer() { // timer runs at 1000 interrupts per second with about a 1ms persicion
    if(ACTIVE_TIMER == TIMER_PIT) {
        init_pit();
    } else if (ACTIVE_TIMER == TIMER_RTC) {

    }
}

void init_pit() {
    asm volatile("cli");
    int divisor = 1193182 / 1000;       /* Calculate our divisor */
    port_byte_out(0x43, 0x36);             /* Set our command byte 0x36 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x40, divisor >> 8);     /* Set high byte of divisor */
    register_interrupt_handler(IRQ0, pit_timer_callback);
    
    asm volatile("sti");
    PIT_ENABLED = 1;
    create_task(get_tcb(3), pit_timer_callback, 0, 0);
    
}

void time_sleep(uint32_t time_ms) {
    sleep_ms = 0;
    sleep_ms_goal = time_ms;
    sleeping = 1;
    if(!PIT_ENABLED) {
        init_pit();
    }

    while(sleeping == 1);

}
