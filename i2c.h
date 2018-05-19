#ifndef _I2C_H
#define _I2C_H

#include <avr/io.h>

//I2C functions
void I2C_init();
void I2C_startCond();
void I2C_stopCond();
void I2C_Write(uint8_t i2cData);
uint8_t I2C_Read(uint8_t ackOpt);

#endif
