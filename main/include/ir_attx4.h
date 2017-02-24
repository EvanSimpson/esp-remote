#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"

#define MODULE_ID_CMD 0x08
#define IR_TX_CMD 0x02

#define PACKET_CONF 0x55
#define FIN_CONF 0x16

#define MODULE_ID 0x0B
#define TX_FREQ 0x26
#define REBOOT_TIME 300

typedef struct {
    uint8_t data[16];
    uint8_t databytes;
} ir_cmd_t;

// Initialize the IR module
void ir_init(spi_device_handle_t spi, int rst_pin);

// Send a short command to the IR module for a response.
void ir_cmd(spi_device_handle_t spi, const uint8_t *data, int len, uint8_t *rx_buffer);

// Send a Transmit command to the IR module
void ir_tx(spi_device_handle_t spi, uint16_t *data, int len);

// Convert an array of 16 bit data into a Tx packet.
void makeTxPacketFrom16BitArray(uint16_t *data, uint8_t length, uint8_t *packet);
