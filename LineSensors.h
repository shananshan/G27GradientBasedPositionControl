#include <Adafruit_SSD1306.h>
#include <splash.h>

#ifndef _LINESENSORS_H
#define _LINESENSORS_H
#define NUM_SENSORS 5
const int sensor_pins[ NUM_SENSORS ] = { A11, A0, A2, A3, A4 };
float threshold = 260;  // 250--greyvalue:56
#define EMIT_PIN   11

class LineSensors_c {
  
  public:
    float readings[ NUM_SENSORS ];
    float minimum[ NUM_SENSORS ];
    float maximum[ NUM_SENSORS ];
    float scaling[ NUM_SENSORS ];
    float calibrated[ NUM_SENSORS ];

    // Constructor, must exist.
    LineSensor_c() {
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
      while (millis() - startTime < 2450) { // 持续4.6/2秒的校准时间
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
      
      // 打印校准值
      for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
        Serial.print("Sensor ");
        Serial.print(sensor);
        Serial.print(": Min = ");
        Serial.print(minimum[sensor]);
        Serial.print(", Max = ");
        Serial.println(maximum[sensor]);
        
        scaling[sensor] = maximum[sensor] - minimum[sensor];
      }
    }

    void calcCalibratedADC() {
      readSensorsADC();
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
//        calibrated[sensor] = (readings[sensor] - minimum[sensor]) / scaling[sensor];
        calibrated[sensor] = (readings[sensor] - minimum[sensor]);
        Serial.print(calibrated[0]);
        Serial.print(",");
        Serial.print(calibrated[1]);
        Serial.print(",");
        Serial.print(calibrated[2]);
        Serial.print(",");
        Serial.print(calibrated[3]);
        Serial.print(",");
        Serial.print(calibrated[4]);
        Serial.print("\n");
      }
    } // End of calcCalibratedADC()

    bool arrived( int sensor ){
      if ( calibrated[sensor] > threshold){
        return true;
      }
      return false;
    }

    float getcalibrated(int sensor){
      float value = calibrated[sensor];
      return value;
    }



/*
 * Digital
 */
    // Part of the Advanced Exercises for Labsheet 2
    


}; // End of LineSensor_c class defintion



#endif
