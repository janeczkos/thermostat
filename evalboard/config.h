#ifndef _CONFIG_H_
#define _CONFIG_H_

#define BUTTON_PORT GPIOA
#define BUTTON_PIN  GPIO1
#define BUTTON_EXTI EXTI1
#define BUTTON_isr  exti1_isr
#define BUTTON_NVIC NVIC_EXTI1_IRQ

#define USING_BMP180

#define USING_DOGM

extern volatile uint32_t button_pressed;

void clock_setup(void);
void i2c_setup(void);
void usart_setup(void);
void ow_usart_setup(void);
void gpio_setup(void);
void encoder_setup(void);
void spi_setup(void);
void lcd_setup(void);
void printLine( uint32_t line_number, char *str );

#endif /*_CONFIG_H_ */
