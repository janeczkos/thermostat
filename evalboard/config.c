#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/f1/exti.h>

#include "config.h"

void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    //led gpio clock
    rcc_periph_clock_enable(RCC_GPIOC);
    //usart gpio clock
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_TIM4);

}

void usart_setup(void)
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

void ow_usart_setup(void)
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


void gpio_setup(void)
{
    //led gpio
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
              GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    //encoder button
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO1);
    nvic_enable_irq(BUTTON_NVIC);
    exti_select_source(BUTTON_EXTI, BUTTON_PORT);
    exti_set_trigger(BUTTON_EXTI, EXTI_TRIGGER_FALLING);
    exti_enable_request(BUTTON_EXTI);
}

volatile uint32_t button_pressed = 0;

void exti1_isr(void)
{
    exti_reset_request(EXTI1);
    button_pressed++;
      
}

encoder_setup(void)
{
        // Use PB6 + PB7 for encoder input (5v)
        gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_TIM4_CH1 );
        gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_TIM4_CH2 );

        timer_reset(TIM4);
        timer_set_period(TIM4,0xffff);
        timer_slave_set_mode(TIM4,TIM_SMCR_SMS_EM1);

        timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
        timer_ic_set_input(TIM4, TIM_IC2, TIM_IC_IN_TI2);
        timer_enable_counter(TIM4);
}
