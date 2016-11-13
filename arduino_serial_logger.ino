/* THIS SKETCH GETS THE DATA FROM THE GALVANIC SKIN RESPONSE SENSOR AND
 SENDS CONDUCTANCE AND RESISTANCE VALUES TO THE SERIAL LINE.
 THERE'S AN "OFFICE EXCEL" PLUGIN TO READ THE DATA FROM THE SERIAL LINE
 AND STORE IT IN A TABLE. */
 
#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h> //FILTER LIBRARIES

#include <eHealth.h>

float conductance, resistance, conductanceVol, ECG;
float ECG_filtered;

float filterFrequency = 5.0  ; // filter out changes faster that 5 Hz.


// create a one pole (RC) lowpass filter
  FilterOnePole lowpassFilter(LOWPASS, filterFrequency);   


void setup() {

Serial.begin(9600); 
 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL, Time, Conductance, Resistance, ECG, ECG_filtered, ...");  //always write LABEL, so excel knows the 
                                                                                //next things will be the names of the columns 
Serial.println("RESETTIMER"); //resets timer to 0 

}

void loop() {
  
  //get the values from the sensors
  
  conductance = eHealth.getSkinConductance();
  resistance = eHealth.getSkinResistance();
  conductanceVol = eHealth.getSkinConductanceVoltage();
  ECG = eHealth.getECG();
 
 //low pass filter on ECG signal
  lowpassFilter.input( ECG ); 

  ECG_filtered = lowpassFilter.output(); 

  // Serial.println(ECG);           //print ECG value for debugging
  // Serial.println(ECG_filtered);  //print ECG_filtered value for debugging
 
 
//send the data to excel
 
  Serial.print("DATA, TIME , "); //writes the time in the first column A and the time since the measurements started in column B
  Serial.print(conductance);  //writes the CONDUCTANCE in the third column
  Serial.print(",");
  Serial.print(resistance);
  Serial.print(",");
  Serial.print(ECG, 2);
   Serial.print(",");
  Serial.print(ECG_filtered, 2);
  Serial.println("\n"); //go into the next row on the next run

  delay(1); //add a delay
}
