#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

//TMP102 thermometer
#define TMP102_ADDR 0x48
#define TMP102_TEMP_REG 0x00

//BMP180 thermometer
#define BMP180ADDR               0x77
#define BMP180_CHIPID_REG        0xD0
#define BMP180_CHIPID_VALUE      0x55

#define BMP180_TAKE_MEAS_REG            0xf4
#define BMP180_READ_MEAS_REG            0xf6
#define BMP180_MEAS_TEMP                0x2e

#define BMP180_EEPROM_AC1       0xaa
#define BMP180_EEPROM_AC2       0xac
#define BMP180_EEPROM_AC3       0xae
#define BMP180_EEPROM_AC4       0xb0
#define BMP180_EEPROM_AC5       0xb2
#define BMP180_EEPROM_AC6       0xb4
#define BMP180_EEPROM_B1        0xb6
#define BMP180_EEPROM_B2        0xb8
#define BMP180_EEPROM_MB        0xba
#define BMP180_EEPROM_MC        0xbc
#define BMP180_EEPROM_MD        0xbe

#define BMP180_EEPROM_NUMREGS ((BMP180_EEPROM_MD-BMP180_EEPROM_AC1+2)/2)

void getTemp_bmp180( uint8_t *pTemperature );
void getTemp_tmp102( uint8_t *pTemperature );

#endif /*_TEMPERATURE_H_ */

