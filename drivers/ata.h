#pragma once

#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../cpu/isr.h"

// Define ATA command codes
#define ATA_CMD_READ_SECTOR  0x20
#define ATA_CMD_WRITE_SECTOR 0x30

// Define device parameters
#define ATA_SECTOR_SIZE      512
#define ATA_SECTOR_COUNT     1024

// Define device registers
#define ATA_REG_DATA         0x1F0
#define ATA_REG_ERROR        0x1F1
#define ATA_REG_FEATURES     0x1F1
#define ATA_REG_SECCOUNT0    0x1F2
#define ATA_REG_LBA0         0x1F3
#define ATA_REG_LBA1         0x1F4
#define ATA_REG_LBA2         0x1F5
#define ATA_REG_DEVSEL       0x1F6
#define ATA_REG_STATUS       0x1F7
#define ATA_REG_COMMAND      0x1F7

// Define device status flags
#define ATA_STATUS_BSY       0x80
#define ATA_STATUS_DRDY      0x40
#define ATA_STATUS_DF        0x20
#define ATA_STATUS_DSC       0x10
#define ATA_STATUS_DRQ       0x08
#define ATA_STATUS_CORR      0x04
#define ATA_STATUS_IDX       0x02
#define ATA_STATUS_ERR       0x01


// Wait for the device to become ready
void ata_wait_ready();

// Send an ATA command to the device
void ata_send_command(uint8_t command, uint8_t features, uint8_t seccount,
                       uint8_t lba_low, uint8_t lba_mid, uint8_t lba_high,
                       uint8_t device);

// Read a sector from the device
void ata_read_sector(uint32_t sector_num, uint8_t* buffer);

// Write a sector to the device
void ata_write_sector(uint32_t sector_num, uint8_t* buffer);