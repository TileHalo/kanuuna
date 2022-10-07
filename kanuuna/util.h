#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

/* Transform four first bits to char of the hex */
char byte_to_hex(uint8_t ch);
uint8_t hex_to_nib(char ch);

#endif
