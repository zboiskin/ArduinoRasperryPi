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

//setting up the switch - switch 1 is associated with pin 2
int Switch1 = 2;

//setting up to establish that the nightlight will be the result or output
void setup() {
  pinMode(NIGHT_LIGHT, OUTPUT); //set up for lightlight as output
  pinMode(Switch1, INPUT);
  Serial.begin(9600);
}

//overall, we are reading the photoresistor pin and displaying the inverse result through the NIGHT_LIGHT
void loop() {
  int lightlevel = analogRead(PHOTORESISTOR_PIN); 
  Serial.println(lightlevel);
  int brightness = map(lightlevel, 0, 1023, 255, 0);
  
if (digitalRead(Switch1) == HIGH) { //if the switch is ON then perform the nightlight function
   analogWrite(NIGHT_LIGHT, brightness);
}
else {
	digitalWrite(NIGHT_LIGHT,LOW); //else if the switch is OFF, turn the light off
}
delay(100);
}