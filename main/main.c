#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

#include "include/ir_attx4.h"
#include "include/onkyo_codes.h"

// Setup pins for SPI output
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

// Specific to Tessel IR module
#define PIN_NUM_RST 18
// Not going to use this one
// Only need it for RX IR data
#define PIN_NUM_IRQ 5

void sendStereoCommand(spi_device_handle_t spi, onkyo_code_t command)
{
  // Create an IR remote command packet
  uint16_t durations_buff[67];
  get_onkyo_code(command, durations_buff);

  // Send the command packet
  ir_tx(spi, durations_buff, 140);
}

void app_main(void)
{
  esp_err_t ret;
  spi_device_handle_t spi;
  // Configure the spi bus and device settings
  spi_bus_config_t buscfg={
    .miso_io_num=PIN_NUM_MISO,
    .mosi_io_num=PIN_NUM_MOSI,
    .sclk_io_num=PIN_NUM_CLK,
    .quadwp_io_num=-1,
    .quadhd_io_num=-1
  };
  spi_device_interface_config_t devcfg={
    .clock_speed_hz=1000,
    .mode=0,
    .spics_io_num=PIN_NUM_CS,
    .queue_size=7
  };
  //Initialize the SPI bus
  ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
  assert(ret==ESP_OK);
  //Attach the IR module to the SPI bus
  ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
  assert(ret==ESP_OK);

  //Initialize the IR module
  ir_init(spi, PIN_NUM_RST);

  // Send a test command to the stereo
  sendStereoCommand(spi, ONKYO_KEY_VOLUMEUP);
  printf("Packet sent.\n");
}
