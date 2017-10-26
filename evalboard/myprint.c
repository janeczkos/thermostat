#include <errno.h>
#include "myprint.h"

int _write(int file, char *ptr, int len)
{
        int i;

        if (file == 1) {
                for (i = 0; i < len; i++)
                        usart_send_blocking(USART2, ptr[i]);
                return i;
        }

        errno = EIO;
        return -1;
}

void DebugPrintDecimal( uint32_t usart, int value )
{
        int8_t i;
        uint8_t nrDigits = 0;
        char buffer[25];

        if (value < 0) {
                usart_send_blocking(usart, '-');
                value = value * -1;
        }

        while (value > 0) {
                buffer[nrDigits++] = "0123456789"[value % 10];
                value /= 10;
        }

        for (i = nrDigits-1; i >= 0; i--)
                usart_send_blocking( usart, buffer[i] );
}

void DebugPrintHex4( uint32_t usart, unsigned char data )
{
	unsigned char character = data & 0x0f;
	if ( character > 9 ) {
		character += 'A' - 10;
	} else {
		character += '0';
	}

	usart_send_blocking( usart, character );
}

void DebugPrintHex8( uint32_t usart, uint8_t data )
{
	DebugPrintHex4( usart, data >> 4 );
	DebugPrintHex4( usart, data );
}

void DebugPrintHex16( uint32_t usart, uint16_t data )
{
	DebugPrintHex8( usart, (uint8_t)(data >> 8) );
	DebugPrintHex8( usart, (uint8_t)data );
}

void DebugPrintHex32( uint32_t usart, uint32_t data )
{
	DebugPrintHex16( usart, (uint16_t)(data >> 16) );
	DebugPrintHex16( usart, (uint16_t)data );
}

/**
 * Send string to USART2
 *
 * @param[in]		str to send
 */
void DebugPrintStr( uint32_t usart, char *str ) 
{
    int   i = 0;

    while(str[i] != 0) {
        usart_send_blocking( usart, str[i++]);
    }
}
