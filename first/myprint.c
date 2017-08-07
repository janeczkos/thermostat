#include "myprint.h"

void UsartPutnumber(uint32_t liczba)
{
        uint8_t setki,dziesiatki;
        setki=liczba/100;
        dziesiatki=(liczba-100*setki)/10;
        usart_send_blocking( USART2, dziesiatki+0x30);
        usart_send_blocking( USART2, liczba-100*setki-10*dziesiatki+0x30);
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
