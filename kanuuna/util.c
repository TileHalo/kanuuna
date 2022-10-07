#include "util.h"

char
byte_to_hex(uint8_t ch)
{
  switch (ch & 0x0F) {
  case 0x00:
    return '0';
  case 0x01:
    return '1';
  case 0x02:
    return '2';
  case 0x03:
    return '3';
  case 0x04:
    return '4';
  case 0x05:
    return '5';
  case 0x06:
    return '6';
  case 0x07:
    return '7';
  case 0x08:
    return '8';
  case 0x09:
    return '9';
  case 0x0A:
    return 'A';
  case 0x0B:
    return 'B';
  case 0x0C:
    return 'C';
  case 0x0D:
    return 'D';
  case 0x0E:
    return 'E';
  case 0x0F:
    return 'F';
  default:
    return 0;
  }
}
