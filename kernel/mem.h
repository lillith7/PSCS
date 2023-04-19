#pragma once

#include <stdint.h>
#include <stddef.h>

#define NULL_POINTER ((void*)0)
#define DYNAMIC_MEM_TOTAL_SIZE 1024*1024 // 1 Megabyte of dynamic memory.
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamic_mem_node_t)

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t nbytes);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

void init_dynamic_mem();

void print_dynamic_node_size();

void print_dynamic_mem();

void *mem_alloc(size_t size);

void mem_free(void *p);

void *mem_realloc(void *prev_mem, size_t old_size, size_t new_size);
