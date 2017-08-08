#include "myprint.h"

void my_usart_print_int(uint32_t usart, int value)
{
        int8_t i;
        uint8_t nr_digits = 0;
        char buffer[25];

        if (value < 0) {
                usart_send_blocking(usart, '-');
                value = value * -1;
        }

        while (value > 0) {
                buffer[nr_digits++] = "0123456789"[value % 10];
                value /= 10;
        }

        for (i = nr_digits-1; i >= 0; i--)
                usart_send_blocking(usart, buffer[i]);
}

void debug_print4( unsigned char data )
{
	unsigned char character = data & 0x0f;
	if (character > 9) {
		character += 'A' - 10;
	} else {
		character += '0';
	}

	usart_send_blocking( USART2, character );
}

void debug_print8(uint8_t data)
{
	debug_print4(data >> 4);
	debug_print4(data);
}

void debug_print16(uint16_t data)
{
	debug_print8((uint8_t)(data >> 8));
	debug_print8((uint8_t)data);
}

void debug_print32(uint32_t data)
{
	debug_print16((uint16_t)(data >> 16));
	debug_print16((uint16_t)data);
}

/**
 * Send string to USART2
 *
 * @param[in]		str to send
 */
void usart_snd_str(char *str) {
    int   i = 0;

    while(str[i] != 0) {
        usart_send_blocking( USART2, str[i++]);
    }
}
