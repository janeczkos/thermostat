#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/f1/exti.h>

#include "config.h"
#include "dogm128.h"

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
    //i2c clock
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
    rcc_periph_clock_enable(RCC_AFIO);}

void i2c_setup(void)
{

	AFIO_MAPR |= AFIO_MAPR_I2C1_REMAP;
	/* Set alternate functions for the SCL and SDA pins of I2C2. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		      GPIO_I2C1_RE_SCL | GPIO_I2C1_RE_SDA);

	/* Disable the I2C before changing any configuration. */
	i2c_peripheral_disable(I2C1);

	/* APB1 is running at 36MHz. */
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);

	/* 100KHz - I2C Standard Mode */
	i2c_set_standard_mode(I2C1);

	/*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
	i2c_set_ccr(I2C1, 0x1e);

	/*
	 * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
	 * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
	 * Incremented by 1 -> 11.
	 */
	i2c_set_trise(I2C1, 0x0b);

	/* If everything is configured -> enable the peripheral. */
	i2c_peripheral_enable(I2C1);
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

void spi_setup(void)
{
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

    /* DC of Nokia 5510 or A0 in DOGM*/
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    /* Reset for lcds */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);

    /* SPI1 clock and MOSI and NSS(CS1) */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO7);

    spi_set_unidirectional_mode(SPI1); /* We want to send only. */
    spi_disable_crc(SPI1); /* No CRC for this slave. */
    spi_set_dff_8bit(SPI1); /* 8-bit dataword-length */
    spi_set_full_duplex_mode(SPI1); /* Not receive-only */
    /* We want to handle the CS signal in software. */
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);
    /* PCLOCK/256 as clock. */
#if defined(USING_DOGM)
	spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_256);
	
#elif defined(USING_NOKIA)
	spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_16);
	
#else
#error LCD not chosen !
#endif 

    /* We want to control everything and generate the clock -> master. */
    spi_set_master_mode(SPI1);
    spi_set_clock_polarity_1(SPI1); /* SCK idle state high. */
    /* Bit is taken on the second (rising edge) of SCK. */
    spi_set_clock_phase_1(SPI1);
    spi_enable_ss_output(SPI1);
    spi_enable(SPI1);
}
void printLine( uint32_t line_number, char *str )
{
	#if defined(USING_DOGM)
	dogm_printLine( line_number, str );
	
#elif defined(USING_NOKIA)
	nokia_printLine( line_number, str );
	
#else
#error LCD not chosen !
#endif 
}
void lcd_setup()
{
	#if defined(USING_DOGM)
	spi_setup();
	dogm128_init();
    dogm128_clear();

	
#elif defined(USING_NOKIA)
	  lcd_init();
	lcd_reset();
	
#else
#error LCD not chosen !
#endif 
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



void encoder_setup(void)
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
