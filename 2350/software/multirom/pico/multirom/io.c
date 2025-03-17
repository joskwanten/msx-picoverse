#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hw_config.h"
#include "multirom.h"
#include "io.h"

void __not_in_flash_func(io_main)(){

    uint8_t  data_buffer[512];

    uint8_t ctr_val = 0;    // Last value written to port 0x9E (control)
    uint8_t out_val = 0;     // Last data response for port 0x9F (data)

    uint16_t data_to_send = 0;
    uint16_t data_to_receive = 0;
    uint16_t data_byte_index = 0;

    uint32_t block_address = 0;
    bool read_address = false;
    
    BYTE const pdrv = 0;  // Physical drive number
    DSTATUS ds = 1; // Disk status (1 = not initialized)

    while (true) {

        bool iorq  = !gpio_get(PIN_IORQ);
        //bool sltsl = !gpio_get(PIN_SLTSL);
        if (iorq)
        { 
            uint32_t gpiostates = gpio_get_all();
            uint8_t busdata = (gpiostates >> 16) & 0xFF;
            uint8_t port = gpiostates & 0xFF;
            
            bool wr = !gpio_get(PIN_WR);
            bool rd = !gpio_get(PIN_RD);
            if (wr)
            {
                if (port == 0x9E) // Port 0x9E (Control Write): Set the control register.
                {
                    // 0x01 = SD card initialization
                    if (busdata == 0x01) 
                    {
                        //printf("MSX: SD card initialization\n");
                        ctr_val = 0xFF;
                        ds = disk_initialize(pdrv);
                        if (ds & STA_NOINIT) 
                        {
                            //printf("Error: SD card initialization failed\n");
                            ctr_val = 0xFF; // Set control register to error state
                        }
                        else 
                        {
                            //printf("SD card initialized successfully\n");
                            ctr_val = 0x00; // Set control register to success state
                        } 
                    }
                
                    // 0x03 = SD card manufacturer ID 
                    if (busdata == 0x03) {
                        if (!(ds & STA_NOINIT)) {
                            sd_card_t *sd_card = sd_get_by_num(0);
                            data_buffer[0] = (uint8_t)ext_bits16(sd_card->state.CID, 127, 120);
                            data_to_send = 1; // 1 byte to send
                            data_byte_index = 0; // Reset index
                            ctr_val = 0x00; // all good
                            //printf("MSX: SD card manufacturer ID: 0x%02X\n", ctrl_reg);
                        }
                        else {
                        // printf("MSX: SD card is not present or not initialized\n");
                        ctr_val = 0xFF; 
                        }
                    }

                    if (busdata == 0x05) 
                    {
                        // Use static variables to keep the state across calls.
                        DWORD capacity = 0;

                        if (!(ds & STA_NOINIT)) {
                            //memset(data_buffer, 0, 4);
                            // set the data_to_send to 4 bytes (32 bits)
                            DRESULT dr = disk_ioctl(pdrv, GET_SECTOR_COUNT, &capacity); // Get the capacity of the SD card
                            //printf("MSX: SD card capacity: %d\n", capacity); 
                            if (dr != RES_OK) {
                                // If there is an error, signal error and reset index.
                                ctr_val = 0xFF;
                                data_to_send = 0;
                                break;
                            }
                            else {
                                //printf("MSX: SD card capacity: %d\n", capacity);
                                data_to_send = 4;        // 4 bytes (32 bits) to send
                                data_byte_index = 0;     // Reset index
                                memcpy(data_buffer, &capacity, 4); // Copy capacity to data buffer
                                ctr_val = 0x00; // all good
                            }
                        }
                        else {
                            // SD card not present or not initialized
                            ctr_val = 0xFF;
                        }
                    }

                    if (busdata == 0x06) 
                    {
                        //printf("Command 0x06: SD card read command\n");
                        if (!(ds & STA_NOINIT)) 
                        {
                            if (!read_address){
                                //printf("Need to get the address in 0x9F\n");
                                data_to_receive = 4; // Set data to receive to 4 bytes (32 bits)
                                data_byte_index = 0; // Reset index
                                ctr_val = 0x01; // receiving state
                                read_address = true; // Set read address flag
                            }
                            else 
                            {
                                read_address = false; // Reset read address flag
                                // Now we have the LBA address
                                block_address = *(uint32_t*)data_buffer; // Read the LBA address from the buffer
                                //printf("Command 0x06: READ SD card read block address: %lu\n", block_address);
                                DRESULT dr = disk_read(pdrv, (BYTE*)data_buffer, block_address, 1);
                                if (dr != RES_OK) {
                                    // If there is an error, signal error and reset index.
                                    ctr_val = 0xFF;
                                    data_to_send = 0;
                                    break;
                                }
                                else {
                                    data_to_send = 512; // Set data to receive to 512 bytes (4096 bits)
                                    data_byte_index = 0; // Reset index
                                    read_address = false; // Reset read address flag
                                    ctr_val = 0x02; // sending state
                                } 
                            }
                            
                        } else 
                        {
                            // SD card not present or not initialized
                            ctr_val = 0xFF;
                        }
                    }

                    // 0x07 = Read the next card block with 512 bytes in size
                    // can only be executed after the 0x06 command
                    if (busdata == 0x07) {
                        if (!(ds & STA_NOINIT)) {
                            //memset(data_buffer, 0, 512);
                            block_address++;
                            //printf("Command 0x07: READ NEXT SD card read block address: %lu\n", block_address);
                            DRESULT dr = disk_read(pdrv, (BYTE *)data_buffer, block_address, 1); // Read one sector from the SD card
                            if (dr != RES_OK)
                            {
                                // If there is an error, signal error and reset index.
                                ctr_val = 0xFF;
                                data_to_send = 0;
                                break;
                            }
                            else
                            {
                                data_to_send = 512;   // Set data to send to 512 bytes (4096 bits)
                                data_byte_index = 0;  // Reset index
                                read_address = false; // Reset read address flag
                                ctr_val = 0x02;       // sending state
                            } 
                        }
                        else {
                            // SD card not present or not initialized
                            ctr_val = 0xFF;
                        }
                    }

                    // 0x08 = Write a 512 byte block to the SD card
                    if (busdata == 0x08) 
                    {
                        if (!(ds & STA_NOINIT)) {
                            if (!read_address){
                                //printf("Need to get the address in 0x9F\n");
                                data_to_receive = 4; // Set data to receive to 4 bytes (32 bits)
                                data_byte_index = 0; // Reset index
                                ctr_val = 0x02; // sending state
                                read_address = true; // Set read address flag
                            }
                            else 
                            {
                                block_address = *(uint32_t*)data_buffer; // Read the LBA address from the buffer
                                read_address = false; // Reset read address flag
                                data_to_receive = 512; // Set data to receive to 512 bytes (4096 bits)
                                data_byte_index = 0; // Reset index
                                //printf("Command 0x08: Need WRITE SD card write block address: %lu\n", block_address);
                            }
                        }
                    }

                     if (busdata == 0x09) 
                     {
                         if (!(ds & STA_NOINIT)) {
                             
                            block_address++;
                            read_address = false; // Reset read address flag
                            data_to_receive = 512; // Set data to receive to 512 bytes (4096 bits)
                            data_byte_index = 0; // Reset index
                            //printf("Command 0x08: Need WRITE SD card write block address: %lu\n", block_address);
                             
                         }
                     }
                }
                else if (port == 0x9F)
                {
                    if (data_to_receive > 0) {
                        // Store the data in the buffer
                        data_buffer[data_byte_index] = busdata;
                        data_byte_index++; // Increment the buffer index
                        data_to_receive--; // Decrement the data to receive
                    }

                    if ((!read_address) && (data_byte_index >=512)) // that means we are not receiving an address and we got a block in the buffer
                    {
                        //printf("BLOCK IS COMPLETE, NEED TO WRITE TO SD CARD\n");
                        DRESULT dr = disk_write(pdrv, (BYTE*)data_buffer, block_address, 1); // Write one sector to the SD card
                        if (dr != RES_OK) {
                            // If there is an error, signal error and reset index.
                            ctr_val = 0xFF;
                            data_to_send = 0;
                        }
                        else
                        {
                            data_byte_index = 0; // Reset index
                        }
                    }
                } 
                // Wait until the write is released.
                while (!gpio_get(PIN_WR)) tight_loop_contents();
            }
            else if (rd)
            {
                // Read transaction: the MSX is reading from the port.
                if (port == 0x9E)
                {
                    gpio_set_dir_out_masked(0xFF << 16); // Set data bus to output mode
                    gpio_put_masked(0xFF0000, ctr_val << 16); // Write the data to the data bus
                    while (!gpio_get(PIN_RD)) tight_loop_contents();
                    gpio_set_dir_in_masked(0xFF << 16); // Return data bus to input mode after cycle completes
                }
                else if (port == 0x9F)
                {
                    if (data_to_send > 0) {
                        // Return the next byte of the data buffer
                        out_val = data_buffer[data_byte_index];
                        data_byte_index++;
                        data_to_send--;
                        
                        gpio_set_dir_out_masked(0xFF << 16); // Set data bus to output mode
                        gpio_put_masked(0xFF0000, out_val << 16); // Write the data to the data bus
                        while (!gpio_get(PIN_RD)) tight_loop_contents();
                        gpio_set_dir_in_masked(0xFF << 16); // Return data bus to input mode after cycle completes

                        ctr_val = 0x02; // sending state
                    }

                    if (data_to_send == 0) { ctr_val = 0x00; } // No more data to send
                    
                }
                while (!gpio_get(PIN_RD)) tight_loop_contents();
            }
        }
    }
}

