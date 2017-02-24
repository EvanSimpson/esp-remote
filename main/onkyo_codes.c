#include "include/onkyo_codes.h"

uint16_t get16BitsComplement(int num)
{
  if ( num < 0 ) {
    return 65536 + num;
  } else {
    return num;
  }
}

void add_one(uint16_t *durations, int position)
{
  durations[position] = ONKYO_ON_DURATION;
  durations[position+1] = get16BitsComplement(-1*ONKYO_ONE_OFF_DURATION);
}

void add_zero(uint16_t *durations, int position)
{
  durations[position] = ONKYO_ON_DURATION;
  durations[position+1] = get16BitsComplement(-1*ONKYO_ZERO_OFF_DURATION);
}

void add_pre_data(uint16_t *durations, int position)
{
  for (uint32_t i=0; i<8; i++){
    if ( (ONKYO_PRE_DATA >> (7 - i)) & 1) {
      add_one(durations, position + (i * 2));
    } else {
      add_zero(durations, position + (i * 2));
    }
  }
}

void add_data(onkyo_code_t command_code, uint16_t *durations, int position)
{
  for (uint32_t i=0; i<24; i++){
    if ( (command_code >> (23 - i)) & 1) {
      add_one(durations, position + (i * 2));
    } else {
      add_zero(durations, position + (i * 2));
    }
  }
}

void add_header(uint16_t *durations, int position)
{
  durations[position] = ONKYO_HEADER_DURATION;
  durations[position+1] = get16BitsComplement(-1*ONKYO_HEADER_SPACE_DURATION);
}

void get_onkyo_code(onkyo_code_t command_code, uint16_t *durations)
{
  // Header start pulse, header space pulse
  add_header(durations, 0);
  // 8 bits of data, 8 on pulses, 8 off pulses
  add_pre_data(durations, 2);
  // 24 bits of data, 24 on pulses, 24 off pulses
  add_data(command_code, durations, 18);
  // ptrail/stop pulse
  durations[66] = ONKYO_ON_DURATION;
}
