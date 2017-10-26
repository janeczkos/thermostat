#include "pcd8544f1.h"


void lcd_init(void)
{
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

    /* DC of Nokia 5510*/
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    /* Reset of Nokia 5510 */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);

    /* pcd8544/SPI1 clock and MOSI and NSS(CS1) */
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
    spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_16);
    /* We want to control everything and generate the clock -> master. */
    spi_set_master_mode(SPI1);
    spi_set_clock_polarity_1(SPI1); /* SCK idle state high. */
    /* Bit is taken on the second (rising edge) of SCK. */
    spi_set_clock_phase_1(SPI1);
    spi_enable_ss_output(SPI1);
    spi_enable(SPI1);
}

void lcd_reset()
{
	uint32_t i,j,k;
	gpio_set( GPIOB, GPIO11 );
	gpio_clear( GPIOB, GPIO12 );
		for (i = 0; i < 100000; i++)	/* Wait a bit. */
			__asm__("nop");
	gpio_set( GPIOB, GPIO12 );
 	
    lcd_send_command(0x21);	

 	lcd_send_command(0x06);	
 	lcd_send_command(0xB0);	
 	lcd_send_command(0x04);	
 	lcd_send_command(0x14);	

 	lcd_send_command(0x20);	
 	lcd_send_command(0x0C);	

	lcd_send_command( 0x80);
	lcd_send_command( 0x40);
	for ( k=0; k<6;k++ ) {
		for (j=0; j<84;j++ ){  
			lcd_send_data(0x00);
		}
	}
}

void lcd_send_command(uint8_t command) 
{
 	while ((SPI_SR(SPI1) & SPI_SR_BSY));
	gpio_clear(GPIOB, GPIO13 );
	spi_send(SPI1,command);
	
}
void lcd_send_data(uint8_t data) 
{
 	while ((SPI_SR(SPI1) & SPI_SR_BSY));
	gpio_set(GPIOB, GPIO13 );
	spi_send(SPI1,data);
}
