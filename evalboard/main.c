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
#include <stdio.h>
#include <errno.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "onewire.h"
#include "myprint.h"
#include "config.h"

int main(void)
{
	int i, c = 0;
 	uint8_t temperature[2];	
    uint32_t pos = 0;
    uint32_t old_pos = 0;

	clock_setup();
	usart_setup();
	gpio_setup();
	ow_usart_setup();
    encoder_setup();

	while (1) {
		/* Using API function gpio_toggle(): */
                
        pos = timer_get_counter(TIM4);
        if ( pos != old_pos ) { 
            gpio_toggle(GPIOC, GPIO13);	/* LED on/off */
            pos = timer_get_counter(TIM4);
            printf("fancy printf: %d\r\n",pos);
            old_pos = pos;
        }
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
