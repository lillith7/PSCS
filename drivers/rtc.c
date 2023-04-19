#include "rtc.h"
#include "ports.h"
#include "../kernel/mem.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
 /*
RTC.c
    With the exception of the commented out inportb and outportb functions, the following code
    was written by Uranium-239 with debugging help from Napalm. The author, Uraninium-239, requests
    that you leave in these two credits.
 
    Please feel free to modify this source.
*/
const char *days_of_week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *months_of_year[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
typedef enum { false, true } bool;
time_t global_time;
bool bcd;
time_t get_global_time() {
    return global_time;
}
const char *get_day_name() {
    return days_of_week[global_time.day_of_week - 1];
    // - 1 because rtc counts starting at 1
}
const char *get_month_name() {
    return months_of_year[global_time.month - 1];
    // -1 because rtc counts starting at 1
}
/*
    If you do not already have the functions inportb and outportb, then use use the examples below
 
unsigned char inportb(unsigned short port)
{
    unsigned char res;
    __asm__ __volatile__("inb %1, %0" : "=a"(res) : "dN"(port));
    return res;
}
 
void outportb(unsigned short port, unsigned char data)
{
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}
*/
 
unsigned char read_register(unsigned char reg)
{
    outportb(0x70, reg);
    return inportb(0x71);
}
 
void write_register(unsigned char reg, unsigned char value)
{
    outportb(0x70, reg);
    outportb(0x71, value);
}
 
unsigned char bcd2bin(unsigned char bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
 
void gettime(time_t *time)
{
    memcpy(time, &global_time, sizeof(time_t));
}
 
/*
struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};
*/
void rtc_handler(registers_t* r)
{
    if(read_register(0x0C) & 0x10){
        if(bcd){
            global_time.second = bcd2bin(read_register(0x00));
            global_time.minute = bcd2bin(read_register(0x02));
            global_time.hour   = bcd2bin(read_register(0x04));
            global_time.month  = bcd2bin(read_register(0x08));
            global_time.year   = bcd2bin(read_register(0x09));
            global_time.day_of_week  = bcd2bin(read_register(0x06));
            global_time.day_of_month = bcd2bin(read_register(0x07));
        }else {
            global_time.second = read_register(0x00);
            global_time.minute = read_register(0x02);
            global_time.hour   = read_register(0x04);
            global_time.month  = read_register(0x08);
            global_time.year   = read_register(0x09);
            global_time.day_of_week  = read_register(0x06);
            global_time.day_of_month = read_register(0x07);
        }
    }
}
 
void rtc_install(void)
{
    unsigned char status;
     
    status = read_register(0x0B);
    status |=  0x02;             // 24 hour clock
    status |=  0x10;             // update ended interrupts
    status &= ~0x20;             // no alarm interrupts
    status &= ~0x40;             // no periodic interrupt
    bcd  =  !(status & 0x04); // check if data type is BCD
    write_register(0x0B, status);
 
    read_register(0x0C);
 
    register_interrupt_handler(IRQ8, rtc_handler);
    /* You may/may not have the above function. It basically installs our RTC handler on IRQ8. If you do not 
    have support for installing interrupt handlers, see this link: http://www.osdever.net/bkerndev/Docs/irqs.htm */
}