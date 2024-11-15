#include <Adafruit_SSD1306.h>
#include <splash.h>
#include "Button.h"

#ifndef _LINESENSORS_H
#define _LINESENSORS_H
#define NUM_SENSORS 5
const int sensor_pins[ NUM_SENSORS ] = { A11, A0, A2, A3, A4 };
float threshold = 0;  // 250--greyvalue:56
#define EMIT_PIN   11

class LineSensors_c {
  
  public:
    float readings[ NUM_SENSORS ];
    float minimum[ NUM_SENSORS ];
    float maximum[ NUM_SENSORS ];
    float scaling[ NUM_SENSORS ];
    float calibrated[ NUM_SENSORS ];
    float error[NUM_SENSORS];

//    // button
//    Button_c maxbutton(A_button);
//    Button_c minbutton(B_button);
//    Button_c desbutton(C_button);
    Button_c button;

    // Constructor, must exist.
    LineSensors_c() {
      // leave this empty
    }

    void initialiseForADC() {
      pinMode( EMIT_PIN, OUTPUT );
      digitalWrite( EMIT_PIN, HIGH );
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
        pinMode(sensor_pins[sensor], INPUT_PULLUP);
      }
      
    } // End of initialiseForADC()

    void readSensorsADC() {
      initialiseForADC();
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
        readings[sensor] = analogRead(sensor_pins[sensor]);
      }
    } // End of readSensorsADC()

    void calibrateSensors() {
      Serial.println("Calibrating sensors, ensure the sensors pass over both the white surface and the black line.");
      for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
        minimum[sensor] = 1023;
        maximum[sensor] = 0;
      }
      unsigned long startTime = millis();
      while (millis() - startTime < 3000) {  //20--5000
        readSensorsADC();
        for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
          if (readings[sensor] < minimum[sensor]) {
              minimum[sensor] = readings[sensor];
          }
          if (readings[sensor] > maximum[sensor]) {
              maximum[sensor] = readings[sensor];
          }
        }
      }
      Serial.println("Sensor 2");
      Serial.print(": Min = ");
      Serial.print(minimum[2]);
      Serial.print(", Max = ");
      Serial.println(maximum[2]);
      // 打印校准值
      for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {        
        scaling[sensor] = maximum[sensor] - minimum[sensor];
        error[sensor] = scaling[sensor]/10000;
      }
    }

    void calcCalibratedADC() {
      readSensorsADC();
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
        calibrated[sensor] = (readings[sensor] - minimum[sensor]) / scaling[sensor];
        // pint
//        calibrated[sensor] = (readings[sensor] - minimum[sensor]);
//        Serial.print(calibrated[0]);
//        Serial.print(",");
//        Serial.print(calibrated[1]);
//        Serial.print(",");
        Serial.println(calibrated[2]);
//        Serial.print(",");
//        Serial.print(calibrated[3]);
//        Serial.print(",");
//        Serial.print(calibrated[4]);
//        Serial.print("\n");
      }
    } // End of calcCalibratedADC()

    float setDestination(){
      Serial.println(threshold);
      while (threshold == 0) {
        calcCalibratedADC();
        while (button.AisPressed()) {
          Serial.println(calibrated[2]);
          threshold = calibrated[2];
          Serial.print("Destination is");
          Serial.println(threshold);
          break;  // Exit the loop once threshold is set
        }
      }
      return threshold;
    }

    bool arrived( int sensor ){
      if ( calibrated[sensor] > threshold-error[sensor] && calibrated[sensor] < threshold+error[sensor]){
        return true;
      }
      return false;
    }

    float getcalibrated(int sensor){
      float value = calibrated[sensor];
      return value;
    }

    float getmax(int sensor){
      float maxvalue = maximum[sensor];
      return maxvalue;
    }

    float getmin(int sensor){
      float minvalue = minimum[sensor];
      return minvalue;
    }

    float getdes(){
      float desvalue = threshold;
      return desvalue;
    }



/*
 * Digital
 */
    void initialiseForDigital() {
      // Ensure that the IR LEDs are on for line sensing
//      pinMode( EMIT_PIN, OUTPUT );
//      digitalWrite( EMIT_PIN, HIGH );
    } // End of initialiseForDigital()

    void readSensorsDigital() {
//      digitalWrite(EMIT_PIN, HIGH);
//      pinMode(sensorPin, OUTPUT);
//      digitalWrite(sensorPin, HIGH);
    } // End of readSensorsDigital()


}; // End of LineSensor_c class defintion



#endif
