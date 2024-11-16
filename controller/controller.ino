#include "LineSensors.h"
//#include "Motors.h"
#include "Button.h"
#include <Pololu3piPlus32U4.h>
#include <math.h>
#include <PololuOLED.h>
#include <EEPROM.h>
#include <Wire.h>

using namespace Pololu3piPlus32U4;

PololuSH1106 display(1, 30, 0, 17, 13);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MOVING (0)
#define STOP (1)

//Motors_c motors;
Motors motors;
LineSensors_c line_sensors;
Button_c button;


unsigned long start_millis = 0;
unsigned long current_real_millis = 0;

// line sensors update
unsigned long set_update_end = 0;
unsigned long set_update_duration = 20;

// speed update
unsigned long set_wheel_duration = 20;
unsigned long set_wheel_end = 0;
float left_wheel = 0.0f;
float right_wheel = 0.0f;
float lspeed = 40.0f;
float rspeed = 40.0f;

unsigned long state = MOVING;

void setup() {
  // put your setup code here, to run once:
  delay(1000);  
  pinMode(LED_BUILTIN, OUTPUT);
  // initialise
  Wire.begin();
//  motors.initialise();
  line_sensors.initialiseForADC();
  button.initialiseForButton();

  // calibrate
  digitalWrite(LED_BUILTIN, HIGH);
//  motors.setPWM(lspeed,rspeed);
  motors.setSpeeds(lspeed,rspeed);
  line_sensors.calibrateSensors();
  digitalWrite(LED_BUILTIN, LOW);
//  motors.setPWM(0,0);
  motors.setSpeeds(0,0);
  line_sensors.setDestination();
  Serial.println("******************************************");

  display.init();
  display.clear();

  // show the calibration data
  displaycali();

  delay(8000);
  display.clear();
  start_millis = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkWheel();
  checkUpdate();
  current_real_millis = millis() - start_millis;
  if (state == MOVING){
    if (line_sensors.arrived(2)){
      displaySensorData();
      Serial.println("Arrived");
      setStop();  
      state = STOP; 
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Not arrived");
      setWheel(lspeed,rspeed);
      displayMoving();
    }
  }else{
    if (button.AisPressed()){
        state = MOVING;
    }
    Serial.println("Unknown state or STOP");
  }
  
//  displaySensorData();
  display.display();
}

void checkUpdate() {
    if (millis() > set_update_end) {
        line_sensors.calcCalibratedADC();
        set_update_end = millis() + set_update_duration;
    }
}

void setStop() {
    setWheel(0, 0);
}

void setWheel(float x, float y) {
    left_wheel = x;
    right_wheel = y;
}

void checkWheel() {
   if (millis() > set_wheel_end) {
//       motors.setPWM(left_wheel, right_wheel);
       motors.setSpeeds(left_wheel, right_wheel);
       setWheel(left_wheel, right_wheel);
       set_wheel_end = millis() + set_wheel_duration;
   }
}
void displayMoving() {
  display.setLayout11x4();
  display.gotoXY(0, 0);
  display.print("sensor2......");
  display.gotoXY(0, 1);
  display.println(line_sensors.getcalibrated(2));
  display.gotoXY(0, 2);
  display.println(line_sensors.getpercentage());
//  display.display();
}
void displaySensorData() {
  
  display.setLayout11x4();
  display.gotoXY(0, 0);
  display.print("sensor2");
  display.gotoXY(0, 1);
  display.println(line_sensors.getcalibrated(2));
  display.gotoXY(0, 2);
  display.println(line_sensors.getpercentage());
//  display.display();
}
void displaycali(){
  display.setLayout11x4();
  display.gotoXY(0, 0);
  display.print("min:");  display.println(line_sensors.getmin(2));
  display.gotoXY(0, 1);
  display.print("max:");  display.println(line_sensors.getmax(2));
  display.gotoXY(0, 2);
  display.print("D:");  display.println(line_sensors.getdes());
  display.gotoXY(0, 3);
  display.print("P:");  display.println(line_sensors.getpercentage());
//  display.display();
}
