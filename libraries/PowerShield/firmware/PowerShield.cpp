// 
// PowerShield.cpp
// Library for the Particle Power Shield
// Based on original library published by Luca Dentella (http://www.lucadentella.it)
// Modified by Mohit Bhoite @ Particle Industries, Inc. July 2015
// Power Shield Documentation: http://docs.particle.io/photon/shields/#power-shield
// MAX17043 datasheet: http://datasheets.maximintegrated.com/en/ds/MAX17043-MAX17044.pdf
//

/* 
         -----[    ]-----
        -|VIN        3V3|-    
        -|GND        RST|-
        -|TX        VBAT|-
        -|RX         GND|-
        -|WKP         D7|-
        -|DAC         D6|-
        -|A5          D5|-
        -|A4          D4|-
        -|A3          D3|- ALERT interrupt from MAX17043 (optional solder bridge)
        -|A2          D2|- 
        -|A1          D1|- SCL |- MAX17043 connected to the I2C channel with address 0x36
        -|A0          D0|- SDA |
          \____________/
 
*/


#include "PowerShield.h"

PowerShield::PowerShield()
{

}

boolean PowerShield::begin()
{
	Wire.begin();
	return 1;
}

// Read and return the cell voltage
float PowerShield::getVCell() {

	byte MSB = 0;
	byte LSB = 0;
	
	readRegister(VCELL_REGISTER, MSB, LSB);
	int value = (MSB << 4) | (LSB >> 4);
	return map(value, 0x000, 0xFFF, 0, 50000) / 10000.0;
	//return value * 0.00125;
}

// Read and return the state of charge of the cell
float PowerShield::getSoC() {
	
	byte MSB = 0;
	byte LSB = 0;
	
	readRegister(SOC_REGISTER, MSB, LSB);
	float decimal = LSB / 256.0;
	return MSB + decimal;	
}

// Return the version number of the chip
int PowerShield::getVersion() {

	byte MSB = 0;
	byte LSB = 0;
	
	readRegister(VERSION_REGISTER, MSB, LSB);
	return (MSB << 8) | LSB;
}

byte PowerShield::getCompensateValue() {

	byte MSB = 0;
	byte LSB = 0;
	
	readConfigRegister(MSB, LSB);
	return MSB;
}

byte PowerShield::getAlertThreshold() {

	byte MSB = 0;
	byte LSB = 0;
	
	readConfigRegister(MSB, LSB);	
	return 32 - (LSB & 0x1F);
}

void PowerShield::setAlertThreshold(byte threshold) {

	byte MSB = 0;
	byte LSB = 0;
	
	readConfigRegister(MSB, LSB);	
	if(threshold > 32) threshold = 32;
	threshold = 32 - threshold;
	
	writeRegister(CONFIG_REGISTER, MSB, (LSB & 0xE0) | threshold);
}

// Check if alert interrupt was generated
boolean PowerShield::getAlert() {

	byte MSB = 0;
	byte LSB = 0;
	
	readConfigRegister(MSB, LSB);	
	return LSB & 0x20;
}

void PowerShield::clearAlert() {

	byte MSB = 0;
	byte LSB = 0;
	
	readConfigRegister(MSB, LSB);	
}

void PowerShield::reset() {
	
	writeRegister(COMMAND_REGISTER, 0x00, 0x54);
}

void PowerShield::quickStart() {
	
	writeRegister(MODE_REGISTER, 0x40, 0x00);
}


void PowerShield::readConfigRegister(byte &MSB, byte &LSB) {

	readRegister(CONFIG_REGISTER, MSB, LSB);
}

void PowerShield::readRegister(byte startAddress, byte &MSB, byte &LSB) {

	Wire.beginTransmission(MAX17043_ADDRESS);
	Wire.write(startAddress);
	Wire.endTransmission();
	
	Wire.requestFrom(MAX17043_ADDRESS, 2);
	MSB = Wire.read();
	LSB = Wire.read();
}

void PowerShield::writeRegister(byte address, byte MSB, byte LSB) {

	Wire.beginTransmission(MAX17043_ADDRESS);
	Wire.write(address);
	Wire.write(MSB);
	Wire.write(LSB);
	Wire.endTransmission();
}
