/*
 * acceleration.c
 *
 *  Created on: Mar 11, 2019
 *      Author: EmilAliyev
 */

//Header include
#include "acceleration.h"

//Common interface includes
#include "i2c_if.h"

//accelerometer definitions
#define ACCELEROMETER_DEVICE_ADDRESS 0x18
#define ACC_X_REG_OFFSET 0x03
#define ACC_Y_REG_OFFSET 0x05

unsigned char xregoffset = ACC_X_REG_OFFSET;
unsigned char yregoffset = ACC_Y_REG_OFFSET;

//Get the x acceleration
int getXAcceleration()
{
    //To hold the acceleration bits
    unsigned char accelerationx[1];

    //Get the acceleration bits for x and y
    I2C_IF_ReadFrom(ACCELEROMETER_DEVICE_ADDRESS, &xregoffset, 1, &accelerationx[0], 1);

    signed char signedaccx = (signed char) accelerationx[0];

    return signedaccx;
}

//Get the y acceleration
int getYAcceleration()
{
    //To hold the acceleration bits
    unsigned char accelerationy[1];

    //Get the acceleration bits for x and y
    I2C_IF_ReadFrom(ACCELEROMETER_DEVICE_ADDRESS, &yregoffset, 1, &accelerationy[0], 1);

    signed char signedaccy = (signed char) accelerationy[0];

    return signedaccy;
}
