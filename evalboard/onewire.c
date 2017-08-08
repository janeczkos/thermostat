#include "onewire.h"


void OW_ReadTemp( uint8_t *temp )
{
	uint8_t rom[8];
	volatile uint32_t i;

	OW_SendByte( 0xCC );
	OW_SendByte( 0xBE );
	for ( i = 0; i < 9; i++ ) {
		rom[i] = OW_ReadByte();
	}
	//ds18b20 13-bit
	// LSB rom[0] 2^3 2^2 2^1 2^1 2^-1 2^-2 2^-3 2^-4
	// MSB rom[1] S S S S S 2^6 2^5 2^4
	temp[0] = ((rom[1]&0x0f)<<4)+((rom[0]&0xf0)>>4);
	temp[1] = (rom[0]&0x0f);
	
	//ds1820 9-bit
	//temp[0] = rom[0]>>1;
	//if ( rom[0] & 1 ) temp[1] = 5;
	//else temp[1] = 0;
	DebugPrintStr( USART2, "\n\rrom: ");
	for ( i = 0; i < 9; i++ ) {
		DebugPrintHex8( USART2, rom[i] );
		usart_send_blocking( USART2, ',');
	}
	
	DebugPrintStr( USART2, "\n\rtemperature: ");
	DebugPrintDecimal( USART2, temp[0] );
	DebugPrintStr( USART2, ",");
	if ( temp[1] > 1  ) {
		DebugPrintDecimal( USART2, (temp[1]*625)/100 );
	} else {
		usart_send_blocking( USART2, '0');
		usart_send_blocking( USART2, '0');
	}
}

uint32_t OW_CheckPresence( void )
{
	int32_t temp= 0;
	usart_set_baudrate( USART1, 9600 );
	usart_send_blocking( USART1, 0xf0 );
	temp = usart_recv_blocking( USART1 );
	usart_set_baudrate( USART1, 115200 );
	if ( temp < 0xf0 ) {
		return 1;
	}
	return 0;
}

void OW_SendByte( uint32_t data )
{
	uint32_t j,temp;
	for ( j = 0; j<8; j++ ) {
		if ( ( data & (1<<j) ) != 0 ) {
			usart_send_blocking( USART1,  0xff );
		} else {
			usart_send_blocking( USART1,  0x00 );
		} 
		temp = usart_recv_blocking( USART1 );
	}
}

uint32_t OW_ReadByte( void )
{
	uint32_t j,temp,data = 0;
	
	for ( j = 0; j<8; j++ ) {
		usart_send_blocking( USART1,  0xff );
		temp = usart_recv_blocking( USART1 );
		if ( temp == 0xff  ) {
			data |= 1<<j;
		} 
	}
	return data;
}

void OW_MeasureTemp( void )
{
	OW_SendByte( 0xCC );
	OW_SendByte( 0x44 );
}
