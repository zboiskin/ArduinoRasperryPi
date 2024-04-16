/* PLAN
We start by including this line of code, which helps our HERO work properly with the Arduino program.
//Night light using day 6 as the concept for as we sense more light, make the light brighter - do the inverse
*/

//Always included
#include "Arduino.h"

//Setting our constants
//A0 is a label specifically for analog reading
//Our photoresistor will connect to this and give us a reading of the current light level 
const byte PHOTORESISTOR_PIN = A0;

//setting the night light pin
const byte NIGHT_LIGHT= 9;

//setting up to establish that the nightlight will be the result or output
void setup() {
  pinMode(NIGHT_LIGHT, OUTPUT);
  Serial.begin(9600);
}

//overall, we are reading the photoresistor pin and displaying the inverse result through the NIGHT_LIGHT
void loop() {
  int lightlevel = analogRead(PHOTORESISTOR_PIN); 
  Serial.println(lightlevel);

  int brightness = map(lightlevel, 0, 1023, 255, 0);
  analogWrite(NIGHT_LIGHT, brightness);

  delay(100);
}