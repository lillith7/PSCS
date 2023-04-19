#pragma once

#include <stdint.h>

unsigned char port_byte_in(uint16_t port);

void port_byte_out(uint16_t port, uint8_t data);

unsigned short port_word_in(uint16_t port);

void port_word_out(uint16_t port, uint16_t data);

/*
 * write a bytes
 * */
void outportb(uint16_t port, uint8_t val);
/*
 * read a byte
 * */
uint8_t inportb(uint16_t port);
/*
 * Read 2 bytes
 * */
uint16_t inports(uint16_t _port);

/*
 * Write 2 bytes
 * */
void outports(uint16_t _port, uint16_t _data);
/*
 * Readt 4 bytes
 * */
uint32_t inportl(uint16_t _port);

/*
 * Write 4 bytes
 * */
void outportl(uint16_t _port, uint32_t _data);