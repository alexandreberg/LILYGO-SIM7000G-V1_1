/*
Jan/2024
Testing the US-100 Ultrasonic Sensor in UART (Serial) Mode
Sensor operation: When the jumper is in place, you use 9600 baud UART to
communicate with the sensor. In UART mode, send 0x55 and read back two
bytes (16 bit value) that is mm distance, or 0x50 to read the temperature in
degrees C. 

Board: LILYGO-SIM7000G-V1_1
Connections:
LilyGo  -   US-100
3V3     -   VCC
IO22    -   Trig/TX
IO21    -   Echo/RX
GND     -   GND
GND     -   GND

Using library: https://github.com/junhuanchen/Esp32-SoftwareSerial.git

Tested ok, results in serial monitor:
Temp: 25 ºC.
Distance: 66 mm
Temp: 25 ºC.
Distance: 66 mm
*/
#include <Arduino.h>
#include <SoftwareSerial.h>
 
const int US100_TX = 22; //define the US100 tx pin
const int US100_RX = 21; //define the US100 rx pin
 
SoftwareSerial US100Serial(US100_RX, US100_TX); //RX, TX
 
unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int mmDist = 0;
int temp = 0;
 
void setup() {
    Serial.begin(115200);
    US100Serial.begin(9600);
}
 
void loop() {
 
    US100Serial.flush();
    US100Serial.write(0x55); //distance
 
    delay(500);
 
    if(US100Serial.available() >= 2) 
    {
        MSByteDist = US100Serial.read(); 
        LSByteDist = US100Serial.read();
        mmDist  = MSByteDist * 256 + LSByteDist; 
        if((mmDist > 1) && (mmDist < 10000)) 
        {
            Serial.print("Distance: ");
            Serial.print(mmDist, DEC);
            Serial.println(" mm");
        }
    }
 
    US100Serial.flush(); 
    US100Serial.write(0x50); //temperature
 
    delay(500);
    if(US100Serial.available() >= 1) 
    {
        temp = US100Serial.read();
        if((temp > 1) && (temp < 130)) // temprature is in range
        {
            temp -= 45; // correct 45º offset
            Serial.print("Temp: ");
            Serial.print(temp, DEC);
            Serial.println(" ºC.");
        }
    }
 
    delay(500);
}