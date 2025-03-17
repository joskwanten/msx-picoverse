#include <stdio.h>
#include "pico/stdlib.h"


int main()
{
    stdio_init_all();

    // Set GPIO28 and GPIO29 to UART function
    gpio_set_function(28, GPIO_FUNC_UART); // TX
    gpio_set_function(29, GPIO_FUNC_UART); // RX

    uart_init(uart0, 859372 ); // Initialize UART with a baud rate of 859372
    uart_set_format(uart0, 8, 1, UART_PARITY_NONE); // Set UART format: 8 data bits, 1 stop bit, no parity
    uart_set_hw_flow(uart0, false, false); // Disable hardware flow control

    sleep_ms(10000); // Wait for 2 seconds
    printf("UART initialized.\n"); // Print message to console
    while (true) {
        printf("Sending to the UART: \n"); // Print message to UART
        uart_puts(uart0, "Hello, UART!"); // Send message over UART
        printf("Waiting for 2 seconds...\n"); // Print message to console
        sleep_ms(2000);
        printf("Reading from the UART: \n"); // Print message to console
        while(uart_is_readable(uart0)) { // Check if data is available to read
            char c = uart_getc(uart0); // Read character from UART
            putchar(c); // Print character to console
        }
        printf("\n"); // Print newline to console
    }
}
