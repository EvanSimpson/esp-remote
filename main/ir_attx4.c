#include "include/ir_attx4.h"

void ir_cmd(spi_device_handle_t spi, const uint8_t *data, int len, uint8_t *rx_buffer)
{
  esp_err_t ret;
  spi_transaction_t t;
  memset(&t, 0, sizeof(t));
  t.length=len*8;
  t.tx_buffer=data;
  t.rx_buffer=rx_buffer;
  ret=spi_device_transmit(spi, &t);
  assert(ret==ESP_OK);
}

void ir_tx(spi_device_handle_t spi, uint16_t *data, int len)
{
  esp_err_t ret;
  spi_transaction_t t;
  memset(&t, 0, sizeof(t));
  uint8_t packet[(len * 2) + 5];
  makeTxPacketFrom16BitArray(data, 67, packet);
  t.length=len*8;
  t.tx_buffer=packet;
  ret=spi_device_transmit(spi, &t);
  assert(ret==ESP_OK);
}

void ir_init(spi_device_handle_t spi, int rst_pin)
{
  // Initialize reset line
  gpio_set_direction(rst_pin, GPIO_MODE_OUTPUT);

  //Reset the module
  gpio_set_level(rst_pin, 0);
  vTaskDelay(100 / portTICK_RATE_MS);
  gpio_set_level(rst_pin, 1);
  vTaskDelay(REBOOT_TIME / portTICK_RATE_MS);

  // send -> MODULE_ID_CMD  = 0x08
  uint8_t init_cmd_packet[3] = {MODULE_ID_CMD, 0x00, 0x00};
  // recieve -> PACKET_CONF, MODULE_ID_CMD, MODULE_ID = 0x55, 0x08, 0x0B
  uint8_t init_rx[3];

  ir_cmd(spi, init_cmd_packet, 3, init_rx);
}

void makeTxPacketFrom16BitArray(uint16_t *data, uint8_t length, uint8_t *packet)
{
  // IR_TX_CMD | TX_FREQ | LENGTH OF DATA | DATA | 0x00 | FIN_CONF
  // 8 bits    | 8 bits  | 8 bits         |      | 8bits| 8bits
  packet[0] = IR_TX_CMD;
  packet[1] = TX_FREQ;
  packet[2] = length;
  for (int i = 0; i<length; i++) {
    packet[3 + (i * 2)] = data[i] >> 8;
    packet[4 + (i * 2)] = data[i] & 0xFF;
  }
  packet[3 + (length * 2)] = 0x00;
  packet[4 + (length * 2)] = FIN_CONF;
}
