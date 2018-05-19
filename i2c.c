#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

/***************************************************************************************************
 *	Function: void I2C_init()
 *	Inputs: none
 *	Outputs: none
 *	Description: This function is used to initialize the I2C module.
 *************************************************************************************************/

void I2C_init()
{
    TWSR = 0x00; //set presca1er bits to zero
    TWBR = 0x46; //SCL frequency is 50K for 16Mhz
    TWCR = 0x04; //enab1e TWI module
}

/***************************************************************************************************
 *	Function: void I2C_Start()
 *	Inputs: none
 *	Outputs: none
 *	Description: This function is used to generate I2C Start Condition.
 *				 Start Condition: SDA goes low when SCL is High.
 *************************************************************************************************/
void I2C_startCond()
{
    TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
    while (!(TWCR & (1 << TWINT)));
}

/***************************************************************************************************
 *	Function: void I2C_Stop()
 *	Inputs: none
 *	Outputs: none
 *	Description: This function is used to generate I2C Stop Condition.
 *				 Stop Condition: SDA goes High when SCL is High.
 *************************************************************************************************/
void I2C_stopCond()
{
    TWCR = ((1 << TWINT) | (1 <<TWEN) | (1 <<TWSTO));
     _delay_us(100) ; //wait for a short time
}

/***************************************************************************************************
 *	Function: void I2C_Write(uint8_t i2cData)
 *	Inputs:  8 bit data to be sent
 *	Outputs: none
 *	Description: This function is used to send a byte on SDA line using I2C protocol bit by bit on 
 *				 each clock cycle. Data sends when SCL is low. MSB sent first, LSB sent last. 
 *************************************************************************************************/
void I2C_Write(uint8_t i2cData)
{
    TWDR = i2cData;
    TWCR = ((1 << TWINT) | (1 <<TWEN));
    while (!(TWCR & (1 << TWINT)));
}

/***************************************************************************************************
 *	Function: void I2C_Read(uint8_t ackOpt)
 *	Inputs:  8 bit data 
 *	Outputs: 8 bit data received 
 *	Description: This fun is used to receive a byte on SDA line using I2C protocol bit by bit on 
 *				 each clock cycle. MSB received first, LSB received last. 
 *************************************************************************************************/
uint8_t I2C_Read(uint8_t ackOpt)
{
    TWCR = ((1 << TWINT) | (1 << TWEN) | (ackOpt << TWEA));
    while ( !(TWCR & (1 << TWINT)));
    return TWDR;
}
