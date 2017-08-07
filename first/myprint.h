#include <libopencm3/stm32/usart.h>

void UsartPutnumber(uint32_t liczba);

void debug_print32(uint32_t data);
void debug_print16(uint16_t data);
void debug_print8(uint8_t data);
void debug_print4(uint8_t data);
void usart_snd_str(char *str);
