int LED1 = 10; //pins 10-12 are to be LED outputs - these correspond to the pins on the HERO board (not the breadboard)
int LED2 = 11;
int LED3 = 12;
int Switch1 = 2; //pins 2-4 are to be switch inputs, one switch to control each of the 3 LEDs
int Switch2 = 3;
int Switch3 = 4;

void setup() {
  //code to assign roles for each variable above, which corresponds to an action for each pin in the HERO board
  //defines input and outputs. Meaning that LED1 etc will be our result, in this case turn on or off and Switch1 will be our input or directions
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(Switch1, INPUT);
pinMode(Switch2, INPUT);
pinMode(Switch3, INPUT);
}

void loop() {
  // now within loop() we'll take actions based on the status of the switches
  //because this is a loop and will continuously cycle through, if we turn a switch on or off, the next time it loops through it will detect the change and execute the else or if
 
 if (digitalRead(Switch1) == HIGH){ //check switch #1 //If Switch1 (which corresponds to pin 2) has voltage flowing through, then turn the light ON (execute code within {})
 digitalWrite(LED1, HIGH); //turn LED on
 }
 else {
  digitalWrite(LED1, LOW); //turn LED off
 }

 if (digitalRead(Switch2) == HIGH){
 digitalWrite(LED2, HIGH);
 }
 else {
  digitalWrite(LED2, LOW);
 }

 if (digitalRead(Switch3) == HIGH){
 digitalWrite(LED3, HIGH);
 }
 else {
  digitalWrite(LED3, LOW);
 }
}