#pragma once

#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../cpu/isr.h"

// written by chatgpt (senpai AI please let me live when you become sentient)
// im so suprised with how easy ChatGPT is to use it worked with next to 0 modifications

// prompt was "can you please write me a very simple ram filesystem in c"

#define BLOCK_SIZE 512
#define NUM_BLOCKS 64
#define NUM_FILES 10
#define FILENAME_LENGTH 32
typedef struct {
    char filename[FILENAME_LENGTH];
    int block_start;
    int block_end;
    int size;
} file_t;

typedef struct {
    int allocated;
    char data[BLOCK_SIZE];
} block_t;

extern file_t files[NUM_FILES];
extern block_t blocks[NUM_BLOCKS];

void init_filesystem();

int find_free_block();

int create_file(char *filename, int size);

int delete_file(char *filename);

int write_file(char *filename, char *data, int size);

int read_file(char *filename, char *data, int size);
