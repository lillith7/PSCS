#include "ata.h"
#include "../drivers/display.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
#include "../cpu/isr.h"

// Define device identification structure
struct ata_identify_device {
    uint16_t  flags;       // General configuration bits
    uint16_t  cylinders;   // Number of logical cylinders
    uint16_t  heads;       // Number of logical heads
    uint16_t  sectors;     // Number of logical sectors per track
    uint8_t   serial[20];  // Drive serial number
    uint16_t  buffertype;  // Buffer type (not used)
    uint16_t  buffersize;  // Buffer size in 512-byte increments
    uint16_t  eccsize;     // ECC block size in bytes
    uint8_t   firmware[8]; // Firmware revision
    uint8_t   model[40];   // Model number
    uint16_t  multiple;    // Maximum number of sectors transferable per interrupt
    uint16_t  capabilities;// Drive capabilities word
    uint16_t  translation;// Obsolete
    uint32_t  sectors48;   // Total number of user addressable sectors (48-bit LBA)
    uint16_t  commandset;  // Command set supported
    uint16_t  features;    // Features supported
    uint16_t  sectorsize;  // User addressable sector size
    uint16_t  sectorspertrack; // Number of sectors per track
    uint16_t  reserved[31];// Reserved for future use
    uint16_t  integrity;   // Integrity word
};

// Define global variables
struct ata_identify_device identify_device;
uint16_t* ata_buffer;

// Wait for the device to become ready
void ata_wait_ready() {
    uint8_t status;
    do {
        status = port_byte_in(ATA_REG_STATUS);
    } while (status & ATA_STATUS_BSY);
}

// Send an ATA command to the device
void ata_send_command(uint8_t command, uint8_t features, uint8_t seccount,
                       uint8_t lba_low, uint8_t lba_mid, uint8_t lba_high,
                       uint8_t device) {
    // Wait for the device to become ready
    ata_wait_ready();

    // Send the command and arguments to the device
    // outb(command, ATA_REG_COMMAND);
    port_byte_out(ATA_REG_COMMAND, command);
    // outb(features, ATA_REG_FEATURES);
    port_byte_out(ATA_REG_FEATURES, features);
    // outb(seccount, ATA_REG_SECCOUNT0);
    port_byte_out(ATA_REG_SECCOUNT0, seccount);
    // outb(lba_low, ATA_REG_LBA0);
    port_byte_out(ATA_REG_LBA0, lba_low);
    // outb(lba_mid, ATA_REG_LBA1);
    port_byte_out(ATA_REG_LBA1, lba_mid);
    // outb(lba_high, ATA_REG_LBA2);
    port_byte_out(ATA_REG_LBA2, lba_high);
    // outb(device, ATA_REG_DEVSEL);
    port_byte_out(ATA_REG_DEVSEL, device);
}

// Read a sector from the device
void ata_read_sector(uint32_t sector_num, uint8_t* buffer) {
    // Calculate the LBA and device parameters
    uint8_t lba_low = sector_num & 0xFF;
    uint8_t lba_mid = (sector_num >> 8) & 0xFF;
    uint8_t lba_high = (sector_num >> 16) & 0xFF;
    uint8_t device = 0xE0;

    // Set the sector count to 1
    uint8_t seccount = 1;

    // Send the ATA read sector command to the device
    ata_send_command(ATA_CMD_READ_SECTOR, 0x0, seccount, lba_low, lba_mid, lba_high, device);

    // Wait for the device to become ready
    ata_wait_ready();

    // Read the data from the device into the buffer
    for (int i = 0; i < ATA_SECTOR_SIZE; i += 2) {
        uint16_t word = port_word_in(ATA_REG_DATA);
        buffer[i] = word & 0xFF;
        buffer[i+1] = (word >> 8) & 0xFF;
    }
}

// Write a sector to the device
void ata_write_sector(uint32_t sector_num, uint8_t* buffer) {
    // Calculate the LBA and device parameters
    uint8_t lba_low = sector_num & 0xFF;
    uint8_t lba_mid = (sector_num >> 8) & 0xFF;
    uint8_t lba_high = (sector_num >> 16) & 0xFF;
    uint8_t device = 0xE0;

    // Set the sector count to 1
    uint8_t seccount = 1;

    // Send the ATA write sector command to the device
    ata_send_command(ATA_CMD_WRITE_SECTOR, 0x0, seccount, lba_low, lba_mid, lba_high, device);

    // Wait for the device to become ready
    ata_wait_ready();

    // Write the data from the buffer to the device
    for (int i = 0; i < ATA_SECTOR_SIZE; i += 2) {
        uint16_t word = buffer[i+1];
        word = (word << 8) | buffer[i];
        // outw(word, ATA_REG_DATA);
        port_word_out(ATA_REG_DATA, word);
    }

    // Flush the write cache
    ata_send_command(0xE7, 0x0, 0x0, 0x0, 0x0, 0x0, device);
    ata_wait_ready();
}
