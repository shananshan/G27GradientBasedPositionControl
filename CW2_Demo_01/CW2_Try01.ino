#include <Pololu3piPlus32U4.h>
#include <Pololu3piPlus32U4IMU.h>
#include <EEPROM.h>
#include <Wire.h>

using namespace Pololu3piPlus32U4;

// 硬件对象
Buzzer buzzer;
LineSensors lineSensors;
Motors motors;
Encoders encoders;
ButtonA buttonA;
ButtonB buttonB;
ButtonC buttonC;
// LCD lcd; // 已去除

// 定义传感器和编码器参数
#define NUM_SENSORS 5
// #define PI 3.1415927
// #define DELTATHETA 1.9197
unsigned int lineSensorValues[NUM_SENSORS];

//参数
int destRead = 0;
int deltaFir = 0;
int deltaSec = 0;

int speed = 40;

// EEPROM 地址
int eepromAddress = 0;


void setup()
{
    // 初始化串口通信
    Serial.begin(9600);

    Wire.begin();
    
    // 播放欢迎音乐
    buzzer.play(">g32>>c32");
    Serial.println("Press B to start");
    // 等待按键B被按下以开始校准
    while(!buttonB.getSingleDebouncedPress());
    // 清空 EEPROM
      for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
      }

    // 校准传感器
    Serial.println("Start calibrate");
    buzzer.play(">g32>>c32");
    calibrateSensors();
    delay(200);
    // 播放欢迎音乐
    buzzer.play(">g32>>c32");
    setDestination();

    // 等待按键B被按下以开始校准
    Serial.println("Press B to run");
    delay(1000);
    while(!buttonB.getSingleDebouncedPress());    
    // 播放开始音乐
    buzzer.play("L16 cdegreg4");
    lineSensors.read(lineSensorValues);
    while(lineSensorValues[2] > destRead){
        moveStright();
    }
    motors.setSpeeds(0, 0);

// *******************************************************
    delay(4000);
    buzzer.play(">g32>>c32>>c32>>g32");
        while(!buttonA.getSingleDebouncedPress());
        Serial.println("Exporting EEPROM Data:");

        int eepromAddress = 0;
        int v1, v2, v3, v4, v5;

        while (eepromAddress + sizeof(v1) + sizeof(v2) + sizeof(v3) + sizeof(v4) + sizeof(v5) <= EEPROM.length()) {
            // 从 EEPROM 中读取 x 和 y 坐标
            EEPROM.get(eepromAddress, v1);
            eepromAddress += sizeof(v1);
            EEPROM.get(eepromAddress, v2);
            eepromAddress += sizeof(v2);
            EEPROM.get(eepromAddress, v3);
            eepromAddress += sizeof(v3);
            EEPROM.get(eepromAddress, v4);
            eepromAddress += sizeof(v4);
            EEPROM.get(eepromAddress, v5);
            eepromAddress += sizeof(v5);

            // 如果读取到了有效数据（可以根据特定的条件来判断）
            if (isnan(v1) || isnan(v2) || isnan(v3) || isnan(v4) || isnan(v5)) {
                break;  // 如果是无效数据，结束读取
            }

            // 通过串口发送坐标数据
            Serial.print("v1: ");
            Serial.print(v1);
            Serial.print("v2: ");
            Serial.print(v2);
            Serial.print("v3: ");
            Serial.print(v3);
            Serial.print("v4: ");
            Serial.print(v4);
            Serial.print("v5: ");
            Serial.println(v5);
        }
        // while(1);

}

void loop()
{
}


// *******************************************************

void moveStright()
{
    lineSensors.read(lineSensorValues);
    savePositionToEEPROM(lineSensorValues[0], lineSensorValues[1], lineSensorValues[2], lineSensorValues[3], lineSensorValues[4]);
    deltaFir = lineSensorValues[0] - lineSensorValues[4];
    deltaSec = lineSensorValues[1] - lineSensorValues[3];

    motors.setSpeeds(speed, speed);
}

// *******************************************************

void calibrateSensors()
{
    // 旋转机器人以进行传感器校准
    delay(500);
    
    for(uint16_t i = 0; i < 40; i++)
    {
        lineSensors.read(lineSensorValues);
        if (i <= 20)
        {
            if(i == 0){
                Serial.println("Press A for White");
                buzzer.play(">g32>>c32"); 
            }
            while(!buttonA.isPressed()){
            }
        }
        else
        {
            if(i == 21){
                Serial.println("Press C for Black");
                buzzer.play(">g32>>c32");
            }
            while(!buttonC.isPressed()){
            }          
        }
        Serial.println(lineSensorValues[2]);
        lineSensors.calibrate();
    }
    motors.setSpeeds(0, 0);
    buzzer.play(">g32>>c32");
}

// *******************************************************

void setDestination()
{
    Serial.println("Press B to set destination");
    while(!buttonB.getSingleDebouncedPress());
    lineSensors.read(lineSensorValues);
    destRead = lineSensorValues[2];
    Serial.println(destRead);
    for(uint16_t i = 0; i < 10; i++)
    {
        lineSensors.read(lineSensorValues);
        destRead = destRead/2 + lineSensorValues[2]/2;
        Serial.println(destRead);
    }
    buzzer.play(">g32>>c32");

}




// *******************************************************
// 保存位置到 EEPROM
void savePositionToEEPROM(int v1, int v2, int v3, int v4, int v5)
{
    if (eepromAddress + sizeof(v1) + sizeof(v2) + sizeof(v3) + sizeof(v4) + sizeof(v5) >= EEPROM.length()) 
    {
        // 如果 EEPROM 空间不足，播放错误提示音
        buzzer.play(">g32>>c32>>c32>>g32");
        return;  // 没有足够的空间
    }

    // 将 v1 坐标保存到 EEPROM
    EEPROM.put(eepromAddress, v1);
    eepromAddress += sizeof(v1);
    // 将 v2 坐标保存到 EEPROM
    EEPROM.put(eepromAddress, v2);
    eepromAddress += sizeof(v2);
    // 将 v3 坐标保存到 EEPROM
    EEPROM.put(eepromAddress, v3);
    eepromAddress += sizeof(v3);
    // 将 v4 坐标保存到 EEPROM
    EEPROM.put(eepromAddress, v4);
    eepromAddress += sizeof(v4);
    // 将 v5 坐标保存到 EEPROM
    EEPROM.put(eepromAddress, v5);
    eepromAddress += sizeof(v5);
}

