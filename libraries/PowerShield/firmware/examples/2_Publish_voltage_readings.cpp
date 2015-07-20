//
// In this example the Particle device publishes the voltage and SoC readings every 10 minutes
// and stays in deep sleep mode for the rest of the time
//

#include "PowerShield/PowerShield.h"

PowerShield batteryMonitor;

void setup() {
  
}

void loop() {    


    Wire.begin(); 
    batteryMonitor.reset();
    batteryMonitor.quickStart();
    delay(1000);
    float cellVoltage = batteryMonitor.getVCell();
    float stateOfCharge = batteryMonitor.getSoC();
    Spark.publish("ps-voltage", String(cellVoltage), 60, PRIVATE);
    delay(100);
    Spark.publish("ps-soc", String(stateOfCharge), 60, PRIVATE);
    delay(100);
    System.sleep(SLEEP_MODE_DEEP, 600);


}