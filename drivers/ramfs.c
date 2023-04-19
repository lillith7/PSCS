#include "ramfs.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../cpu/isr.h"

// written by chatgpt (senpai AI please let me live when you become sentient)
// im so suprised with how easy ChatGPT is to use it worked with next to 0 modifications

// prompt was "can you please write me a very simple ram filesystem in c"



file_t files[NUM_FILES];
block_t blocks[NUM_BLOCKS];

void init_filesystem() {
    int i;
    for (i = 0; i < NUM_FILES; i++) {
        files[i].filename[0] = '\0';
        files[i].block_start = -1;
        files[i].block_end = -1;
        files[i].size = 0;
    }
    for (i = 0; i < NUM_BLOCKS; i++) {
        blocks[i].allocated = 0;
        // memset(blocks[i].data, 0, BLOCK_SIZE);
        // for (uint16_t j = 0; j < BLOCK_SIZE; j++) {
        //     blocks[i].data[j] = 0;
        // }
    }
}

int find_free_block() {
    int i;
    for (i = 0; i < NUM_BLOCKS; i++) {
        if (!blocks[i].allocated) {
            return i;
        }
    }
    return -1;
}

int create_file(char *filename, int size) {
    int i, block, blocks_needed;
    for (i = 0; i < NUM_FILES; i++) {
        if (files[i].filename[0] == '\0') {
            blocks_needed = size / BLOCK_SIZE;
            if (size % BLOCK_SIZE != 0) {
                blocks_needed++;
            }
            block = find_free_block();
            if (block == -1) {
                return -1;
            }
            files[i].block_start = block;
            files[i].block_end = block + blocks_needed - 1;
            strcpy(files[i].filename, filename);
            files[i].size = size;
            blocks[block].allocated = 1;
            if (blocks_needed > 1) {
                for (int j = 1; j < blocks_needed; j++) {
                    block = find_free_block();
                    if (block == -1) {
                        return -1;
                    }
                    blocks[block].allocated = 1;
                }
            }
            return i;
        }
    }
    return -1;
}

int delete_file(char *filename) {
    int i;
    for (i = 0; i < NUM_FILES; i++) {
        if (compare_string(files[i].filename, filename) == 0) {
            for (int j = files[i].block_start; j <= files[i].block_end; j++) {
                blocks[j].allocated = 0;
                memset(blocks[j].data, 0, BLOCK_SIZE);
            }
            files[i].filename[0] = '\0';
            files[i].block_start = -1;
            files[i].block_end = -1;
            files[i].size = 0;
            return 0;
        }
    }
    return -1;
}

int write_file(char *filename, char *data, int size) {
    int i;
    for (i = 0; i < NUM_FILES; i++) {
        if (compare_string(files[i].filename, filename) == 0) {
            if (size > files[i].size) {
                return -1;
            }
            int current_block = files[i].block_start;
            int data_offset = 0;
            int data_remaining = size;
            while (data_remaining > 0) {
                int bytes_to_write = data_remaining > BLOCK_SIZE ? BLOCK_SIZE : data_remaining;
                memcpy(blocks[current_block].data, &data[data_offset], bytes_to_write);
                data_offset += bytes_to_write;
                data_remaining -= bytes_to_write;
                current_block++;
            }
        return 0;
        }
    }
    return -1;
}

int read_file(char *filename, char *data, int size) {
    int i;
    for (i = 0; i < NUM_FILES; i++) {
        if (compare_string(files[i].filename, filename) == 0) {
            if (size < files[i].size) {
                return -1;
            }
            int current_block = files[i].block_start;
            int data_offset = 0;
            int data_remaining = files[i].size;
            while (data_remaining > 0) {
                int bytes_to_read = data_remaining > BLOCK_SIZE ? BLOCK_SIZE : data_remaining;
                memcpy(&data[data_offset], blocks[current_block].data, bytes_to_read);
                data_offset += bytes_to_read;
                data_remaining -= bytes_to_read;
                current_block++;
            }
            return 0;
        }
    }
    return -1;
}
