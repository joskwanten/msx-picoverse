
#define PORT_CONTROL   0x9E //PORTCFG 
#define PORT_DATAREG   0x9F //PORTSPI

void spi_initialize();
uint8_t spi_handle_control_register();
void io_main();
