#pragma once
#include "shell.h"
#include "defines.h"

#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/power.h"

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

#define CMD_FINISH print_string("\n> ")

void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("cli");
        asm volatile("hlt");
        return;
    } else if (compare_string(input, "RESET") == 0 || compare_string(input, "RESTART") == 0) {
        power_reset();
        return;
    } else if (compare_string(input, "SHUTDOWN") == 0) {
        power_shutdown();
        CMD_FINISH;
        return;
    } else if (compare_string(input, "UPTIME") == 0) {
        #ifndef TIMER_ENABLED

        print_string("Timer not compiled.\n");
        CMD_FINISH;
        
        #else
 
        print_string("UPTIME: ");
        char* secs[4];
        char* mins[4];
        char* hours[24];
        int_to_string(get_timer_secs(), secs);
        int_to_string(get_timer_mins(), mins);
        int_to_string(get_timer_hours(), hours);
        print_string(hours);
        print_string(" Hours, ");
        print_string(mins);
        print_string(" Minutes, and ");
        print_string(secs);
        print_string(" Seconds.");
        CMD_FINISH;
        #endif

        return;
    } else if (compare_string(input, "LS") == 0) {
        #ifndef FS_ENABLED
        print_string("Filesystem not compiled.\n");
        CMD_FINISH;
        #else
        print_string("Filesystem Listing.\n");
        for (uint16_t i = 0; i < NUM_FILES; i++) {
            if(files[i].filename[0] != 0) {
                print_string(files[i].filename);
                print_nl();
            }
        }
        CMD_FINISH;
        #endif
        return;
    } else if (compare_string(input, "TEXT") == 0) {
        #ifndef BUILT_IN_TEXT_EDITOR_ENABLED
        print_string("Built-in Text editor not compiled.");
        CMD_FINISH;
        #else
        print_string("Starting text editor..\n");
        text_editor_init();
        #endif
        return;
    } else if(compare_string(input, "RTC") == 0) {

        #ifndef RTC_ENABLED
        print_string("RTC supported but not compiled.");
        #else

        print_string("RTC:\n");
        char time_inp[20];
        int_to_string(get_global_time().year, time_inp);
        print_string("year: ");
        print_string("20");
        print_string(time_inp);
        print_nl();
        print_string("month: ");
        print_string(get_month_name());
        print_nl();
        int_to_string(get_global_time().day_of_month, time_inp);
        print_string("day of month: ");
        print_string(time_inp);
        print_nl();
        print_string("day of week: ");
        print_string(get_day_name());
        print_nl();
        int_to_string(get_global_time().hour, time_inp);
        print_string("hour: ");
        print_string(time_inp);
        print_nl();
        int_to_string(get_global_time().minute, time_inp);
        print_string("minute: ");
        print_string(time_inp);
        print_nl();
        int_to_string(get_global_time().second, time_inp);
        print_string("second: ");
        print_string(time_inp);
        print_nl();
        #endif
        CMD_FINISH;
        return;
    }

    print_string("\nUnknown command: ");
    print_string(input);
    print_nl();
    print_string("> ");
}
