#include "LineSensors.h"
#include "Motors.h"
#include <math.h>
#include <PololuOLED.h>

PololuSH1106 display(1, 30, 0, 17, 13);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MOVING (1)
#define STOP (2)

Motors_c motors;
LineSensors_c line_sensors;

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
float lspeed = 19.0f;
float rspeed = 20.0f;

// threshold
//float threshold = 128;

unsigned long state = MOVING;

void setup() {
  // put your setup code here, to run once:
  delay(1000);  
  pinMode(LED_BUILTIN, OUTPUT);
  // initialise
  motors.initialise();
  line_sensors.initialiseForADC();
  display.init();
  display.clear();
  display.print("Hello, OLED!"); 
  
  // calibrate
  motors.setPWM(-lspeed, rspeed);
  digitalWrite(LED_BUILTIN, HIGH);
  line_sensors.calibrateSensors();
  motors.setPWM(lspeed, -rspeed);
  line_sensors.calibrateSensors();
  motors.setPWM(0,0);
  digitalWrite(LED_BUILTIN, LOW);

  delay(1000);
  start_millis = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkWheel();
  checkUpdate();
  current_real_millis = millis() - start_millis;
  if (line_sensors.arrived(2)){
    digitalWrite(LED_BUILTIN, HIGH);
    setStop();   
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    setWheel(lspeed,rspeed);
  }
  
  displaySensorData();
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

void setForward() {
    setWheel(lspeed, rspeed);
}


void setWheel(float x, float y) {
    left_wheel = x;
    right_wheel = y;
}

void checkWheel() {
   if (millis() > set_wheel_end) {
       motors.setPWM(left_wheel, right_wheel);
       setWheel(left_wheel, right_wheel);
       set_wheel_end = millis() + set_wheel_duration;
   }
}

void displaySensorData() {
  display.gotoXY(0, 0);
  display.print("sensor 2");
  display.gotoXY(0, 1);
  display.println(line_sensors.getcalibrated(2));
  display.display();
}
