// Library to make our Arduino work with our HERO board
#include "Arduino.h"

 //#define is the command we use when we want to give a constant a nickname 
 //In this case, rather than remembering that pin 12 is our cabin light, let's just call it CABIN_LIGHTS_PIN name with the value of 12 (our pin)
 //using #define is specifically for naming a constant, meaning we cannot change it's name in the program

#define CABIN_LIGHTS_PIN 12   // This is the nickname for the current pin 12 which is connected to our LED.


//The setup function is where we prepare everything before we start. We only run this function once
//Here, we are setting up our CABIN_LIGHTS_PIN as an output pin, so we can control our light with it.
 
void setup() {
  pinMode(CABIN_LIGHTS_PIN, OUTPUT);    // Here we are specifying that our cabin light is an output, or result of our code.
}

//After the setup function, the loop function starts
//Here, we are making our light turn on and off in a repeating pattern.
void loop() {
  digitalWrite(CABIN_LIGHTS_PIN, HIGH);  // This line turns the lED light ON.
  delay(1000);                           // Wait for one second (1000 milliseconds) with the light ON.
  digitalWrite(CABIN_LIGHTS_PIN, LOW);   // Turn the light OFF.
  delay(100);                            // Wait for a tenth of a second (100 milliseconds) with the light OFF.
										 // And because this is in a loop, it will run through again and repeat
}