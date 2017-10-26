#ifndef MYPRINT_H
#define MYPRINT_H

#include <libopencm3/stm32/usart.h>

void DebugPrintDecimal( uint32_t usart, int value );
void DebugPrintStr( uint32_t usart, char *str );

void DebugPrintHex32( uint32_t usart, uint32_t data );
void DebugPrintHex16( uint32_t usart, uint16_t data );
void DebugPrintHex8( uint32_t usart, uint8_t data );
void DebugPrintHex4( uint32_t usart, uint8_t data );

int _write(int file, char *ptr, int len);

#endif //MYPRINT_H
