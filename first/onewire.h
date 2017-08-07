#include <libopencm3/stm32/usart.h>
#include "myprint.h"

uint32_t OW_ReadByte( void );
void OW_SendByte( uint32_t data );

uint32_t OW_CheckPresence( void );

void OW_MeasureTemp( void );
void OW_ReadTemp( uint8_t *temp );
		
