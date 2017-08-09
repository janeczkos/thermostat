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

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "onewire.h"
#include "myprint.h"

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	//led gpio clock
	rcc_periph_clock_enable(RCC_GPIOC);
	//usart gpio clock
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_USART1);
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
static void ow_usart_setup(void)
{
        /* Setup GPIO pin GPIO_USART2_TX. */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                      GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

        /* Setup UART parameters. */
        usart_set_baudrate(USART1, 115200);
        usart_set_databits(USART1, 8);
        usart_set_stopbits(USART1, USART_STOPBITS_1);
        usart_set_mode(USART1, USART_MODE_TX_RX);
        usart_set_parity(USART1, USART_PARITY_NONE);
        usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

        /* Finally enable the USART. */
        usart_enable(USART1);
}


static void gpio_setup(void)
{
	//led gpio
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

static void encoder_setup(void)
{
     // Use PB6 + PB7 for encoder input (5v)
             gpio_set_mode    (GPIOB, GPIO_MODE_INPUT,
          GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6);
               gpio_set_mode    (GPIOB, GPIO_MODE_INPUT,
          GPIO_CNF_INPUT_PULL_UPDOWN, GPIO7);
                timer_reset(TIM4);
             rcc_periph_clock_enable(RCC_TIM4);
             timer_set_period(TIM4, 0xFFFF);
             timer_slave_set_mode(TIM4, TIM_SMCR_SMS_EM3); // encoder
     
             timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
             timer_ic_set_input(TIM4, TIM_IC2, TIM_IC_IN_TI2);
             timer_enable_counter(TIM4);
}
int main(void)
{
	int i, c = 0;
 	uint8_t temperature[2];	
    uint32_t pos = 0;

	clock_setup();
	usart_setup();
	gpio_setup();
	ow_usart_setup();
    encoder_setup();

	while (1) {
		/* Using API function gpio_toggle(): */
		gpio_toggle(GPIOC, GPIO13);	/* LED on/off */
        pos = timer_get_counter(TIM4);
        for (i = 0; i < 8000000; i++)_asm__("nop");
        DebugPrintStr( USART2, "pozycja enkodera: ");
        DebugPrintDecimal( USART2, pos); 
		usart_send_blocking( USART2, '\r' );
        usart_send_blocking( USART2, '\n' );
		/*if ( OW_CheckPresence() ) {
                        OW_MeasureTemp();
                }

                for (i = 0; i < 8000000; i++)   
                        __asm__("nop");
                for (i = 0; i < 8000000; i++)   
                        __asm__("nop");
		if ( OW_CheckPresence() ) {
                        OW_ReadTemp( temperature );
                }*/
	}

	return 0;
}
