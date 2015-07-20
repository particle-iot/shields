// 
// PowerShield.h
// Library for the Particle Power Shield
// Based on original library published by Luca Dentella (http://www.lucadentella.it)
// Modified by Mohit Bhoite @ Particle Industries, Inc. July 2015
// Power Shield Documentation: http://docs.particle.io/photon/shields/#power-shield
// MAX17043 datasheet: http://datasheets.maximintegrated.com/en/ds/MAX17043-MAX17044.pdf
//

#include "application.h"

#ifndef _PowerShield_H
#define _PowerShield_H

//Default MAX17043 I2C address
#define MAX17043_ADDRESS	0x36

// Register definitions
#define VCELL_REGISTER		0x02
#define SOC_REGISTER		0x04
#define MODE_REGISTER		0x06
#define VERSION_REGISTER	0x08
#define CONFIG_REGISTER		0x0C
#define COMMAND_REGISTER	0xFE


class PowerShield {

	public:
	    PowerShield();
		boolean begin();
		float getVCell();
		float getSoC();
		int getVersion();
		byte getCompensateValue();
		byte getAlertThreshold();
		void setAlertThreshold(byte threshold);
		boolean getAlert();
		void clearAlert();
		void reset();
		void quickStart();
	
	private:

		void readConfigRegister(byte &MSB, byte &LSB);
		void readRegister(byte startAddress, byte &MSB, byte &LSB);
		void writeRegister(byte address, byte MSB, byte LSB);
};

#endif