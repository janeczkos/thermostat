/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	//led gpio clock
	rcc_periph_clock_enable(RCC_GPIOC);
	//usart gpio clock
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void)
{
        /* Setup GPIO pin GPIO_USART2_TX. */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);

        /* Setup UART parameters. */
        usart_set_baudrate(USART2, 115200);
        usart_set_databits(USART2, 8);
        usart_set_stopbits(USART2, USART_STOPBITS_1);
        usart_set_mode(USART2, USART_MODE_TX);
        usart_set_parity(USART2, USART_PARITY_NONE);
        usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

        /* Finally enable the USART. */
        usart_enable(USART2);
}

static void my_usart_print_int(uint32_t usart, int value)
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

static void gpio_setup(void)
{
	//led gpio
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

int main(void)
{
	int i, c = 0;

	clock_setup();
	usart_setup();
	gpio_setup();

	while (1) {
		/* Using API function gpio_toggle(): */
		gpio_toggle(GPIOC, GPIO13);	/* LED on/off */
		my_usart_print_int(USART2,c++);
		usart_send_blocking(USART2, '\r');
                usart_send_blocking(USART2, '\n');
                for (i = 0; i < 4000000; i++)   /* Wait a bit. */
                        __asm__("nop");
	}

	return 0;
}
