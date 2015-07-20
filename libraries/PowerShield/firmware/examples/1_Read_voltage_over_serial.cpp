//
// In this example the Particle device sends the volatge and SoC readings over serial
// every 1 second
//

#include "PowerShield/PowerShield.h"

PowerShield batteryMonitor;

void setup() {
    
    Serial.begin(9600); 
    // This essentially starts the I2C bus
    batteryMonitor.begin(); 
    // This sets up the fuel gauge
    batteryMonitor.quickStart();
    // Wait for it to settle down
    delay(500);

}

void loop() {
    
    // Read the volatge of the LiPo
    float cellVoltage = batteryMonitor.getVCell();
    // Read the State of Charge of the LiPo
    float stateOfCharge = batteryMonitor.getSoC();
    
    // Send the Voltage and SoC readings over serial
    Serial.println(cellVoltage);
    Serial.println(stateOfCharge);
    delay(1000);

}