#include <stdint.h>

/**
 * Read a byte from the specified port
 */
unsigned char port_byte_in(uint16_t port) {
    unsigned char result;
    /* Inline assembler syntax
     * !! Notice how the source and destination registers are switched from NASM !!
     *
     * '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
     * '"d" (port)': map the C variable '(port)' into e'd'x register
     *
     * Inputs and outputs are separated by colons
     */
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data) {
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the asm syntax
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(uint16_t port) {
    unsigned short result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/*
 * write a bytes
 * */
void outportb(uint16_t port, uint8_t val) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}

/*
 * read a byte
 * */
uint8_t inportb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/*
 * Read 2 bytes
 * */
uint16_t inports(uint16_t _port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/*
 * Write 2 bytes
 * */
void outports(uint16_t _port, uint16_t _data) {
    asm volatile ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

/*
 * Readt 4 bytes
 * */
uint32_t inportl(uint16_t _port) {
    uint32_t rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (_port));
    return rv;
}

/*
 * Write 4 bytes
 * */
void outportl(uint16_t _port, uint32_t _data) {
    asm volatile ("outl %%eax, %%dx" : : "dN" (_port), "a" (_data));
}