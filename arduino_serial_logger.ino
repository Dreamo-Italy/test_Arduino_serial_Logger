/* THIS SKETCH GETS THE DATA FROM THE GALVANIC SKIN RESPONSE SENSOR AND
 SENDS CONDUCTANCE AND RESISTANCE VALUE TO THE SERIAL LINE.
 THEN THERE'S AN OFFICE' EXCEL PLUGIN TO READ THE DATA FROM THE SERIAL LINE
 AND STORE IT IN A TABLE. */
 
#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h> //FILTERS LIBRARY

#include <eHealth.h>

float conductance, resistance, conductanceVol, ECG;
float ECG_filtered;
// filters out changes faster that 5 Hz.
float filterFrequency = 5.0  ;


// create a one pole (RC) lowpass filter
  FilterOnePole lowpassFilter(LOWPASS, filterFrequency);   


void setup() {

Serial.begin(9600); 
 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
Serial.println("CLEARDATA"); //clears up any data left from previous projects

Serial.println("LABEL, Time, Conductance, Resistance, ECG, ECG_filtered, ..."); //always write LABEL, so excel knows the 
//next things will be the names of the columns (instead of Acolumn you could write Time for instance)

Serial.println("RESETTIMER"); //resets timer to 0

 // establishContact();  // send a byte to establish contact until receiver responds
 

}

void loop() {
  
  //get the values from the sensors
  
  conductance = eHealth.getSkinConductance();
  resistance = eHealth.getSkinResistance();
  conductanceVol = eHealth.getSkinConductanceVoltage();
  ECG = eHealth.getECG();
 
  lowpassFilter.input( ECG ); //low pass filter on ECG signal

  ECG_filtered = lowpassFilter.output(); 

  //Serial.println(ECG);

  Serial.print("DATA, TIME , "); //writes the time in the first column A and the time since the measurements started in column B
  Serial.print(conductance); //INSERT THE DATA
  Serial.print(",");
  Serial.print(resistance); //INSERT THE DATA
  Serial.print(",");
  Serial.print(ECG, 2);
   Serial.print(",");
  Serial.print(ECG_filtered, 2);
  Serial.println("\n"); //be sure to add println to the last command so it knows to go into the next row on the second run

  delay(1); //add a delay
}
