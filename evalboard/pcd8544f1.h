#ifndef PCD8544F1_H
#define PCD8544F1_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>

void lcd_init(void);
void lcd_reset(void);
void lcd_send_command(uint8_t command);
void lcd_send_data(uint8_t data);

#endif /* PCD8544F1_H */
