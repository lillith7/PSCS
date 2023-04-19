#include "../drivers/ports.h"

void power_reset(void) {
    unsigned short good = 0x02;
    while (good & 0x02)
        good = port_byte_in(0x64);
    port_byte_out(0x64, 0xFE);
    asm volatile("hlt");
}

void power_shutdown(void) { // emulator only currently bc acpi is too complicated
    port_word_out(0xB004, 0x2000); // bochs
    port_word_out(0x604, 0x2000); // qemu
    port_word_out(0x4004, 0x3400); // virtualbox
}
