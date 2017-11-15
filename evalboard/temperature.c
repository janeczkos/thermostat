#include <libopencm3/stm32/i2c.h>
#include <stdio.h>

#include "temperature.h"
#include "config.h"

void getTemp_tmp102( uint8_t *pTemperature )
{
	uint8_t iCmd2Send;
	
	
	iCmd2Send = TMP102_TEMP_REG;
	i2c_transfer7( I2C1, TMP102_ADDR, &iCmd2Send, 1, pTemperature, 2);
}




static int readEEPROM_bmp180( int eeprom[BMP180_EEPROM_NUMREGS] )
{
	uint8_t buff[2*BMP180_EEPROM_NUMREGS] ;
	int bytesRead ;
	int i, j ;
	uint8_t data;
	
	data = BMP180_EEPROM_AC1;
	i2c_transfer7( I2C1, BMP180ADDR, &data, 1, buff, 2*BMP180_EEPROM_NUMREGS ) ;

	for (i=0, j=0 ; i<2*BMP180_EEPROM_NUMREGS; i+=2, ++j) {
		int bms, bls ;
		bms = (signed char)buff[i] ;
		bls = buff[i+1] ;
		eeprom[j] = bms*256 + bls ;
	}
	return j ;

}

int readBEWord( uint8_t addr ) {
  int  bms, bls ;
  uint8_t data;
  uint8_t dataRead[2];
  
  
  i2c_transfer7( I2C1, BMP180ADDR, &addr, 1, dataRead, 2 ) ;
  
  bms = dataRead[0];
  bls = dataRead[1] ;
  
  return bms*256 + bls ;
}


void getTemp_bmp180( uint8_t *pTemperature )
{
	//int i2cfd ;
	int dataFromDevice ;
	int rc ;
	uint8_t data[2];
	uint32_t i;
	
	int eePROM[BMP180_EEPROM_NUMREGS] ;
	/*
	* These are the values mentioned on p 15.
	* Only a few of them are used to compute the temperature.
	*/
	/* EEPROM registers */
	int ac1, ac2, ac3, ac4, ac5, ac6, b1, b2, mb, mc, md ;
	/* Needed in the computation */
	int b3, b4, b5, b6, b7, x1, x2 ;

	/* The results */
	int ut, t ;

	double c ;
	
	rc = readEEPROM_bmp180( eePROM );
	if ( rc != BMP180_EEPROM_NUMREGS ) {
		printf("not enought EEPROM\r\n");
		return ;
	}
	
	ac5 = eePROM[(BMP180_EEPROM_AC5 - BMP180_EEPROM_AC1)/2] ;
	ac6 = eePROM[(BMP180_EEPROM_AC6 - BMP180_EEPROM_AC1)/2] ;
	mc  = eePROM[(BMP180_EEPROM_MC  - BMP180_EEPROM_AC1)/2] ;
	md  = eePROM[(BMP180_EEPROM_MD  - BMP180_EEPROM_AC1)/2] ;

	data [0] = BMP180_TAKE_MEAS_REG;
	data [1] = BMP180_MEAS_TEMP;
	i2c_transfer7( I2C1, BMP180ADDR, data, 2, pTemperature, 0);
	
	for (i = 0; i < 100000; i++)   
                        __asm__("nop");
	
	ut = readBEWord( BMP180_READ_MEAS_REG ) ;
	x1 = (ut-ac6)*ac5/32768 ;
	x2 = mc * 2048 / (x1 + md) ;
	b5 = x1 + x2 ;
	t = (b5 + 8) / 16 ;
	/*
	printf("ut  = %6d\r\n", ut) ;
	printf("ac5 = %6d\r\n", ac5) ;
	printf("ac6 = %6d\r\n", ac6) ;
	printf("mc  = %6d\r\n", mc) ;
	printf("md  = %6d\r\n", md) ;
	printf("x1  = %6d\r\n", x1) ;
	printf("x2  = %6d\r\n", x2) ;
	printf("b5  = %6d\r\n", b5) ;
	printf("t   = %6d\r\n", t) ;
	printf("\r\n");*/
	
	pTemperature[0] = t/10;
	pTemperature[1] = t%10;
}

void getTemperature( uint8_t *pTemperature )
{
#if defined(USING_TMP102)
	getTemp_tmp102( pTemperature );
#elif defined(USING_BMP180)
	getTemp_bmp180( pTemperature );
#else
	#error Temperature sensor not chosen !
#endif 
}

