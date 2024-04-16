int LED = 12;
int Switch1 = 2; //pin 2 will be attached to our switch - this variable controls the switch
void setup() {
  //setup both an output and an input on the HERO:
  pinMode(LED, OUTPUT); //we are saying that we want our LED light with is equal to 12 (because it is in pin 12) to be our output result in this case turn on or off
  pinMode(Switch1, INPUT); //this is saying that our HERO board is going to watch and take cues from our switch to take our desired action

}

void loop() {
//now within loop() we'll take actions based on the status of the input switch

//this is a conditional test...
//in plain English - if we use the function digitalRead (which senses voltage in a pin) and sense voltage in our pin 2, then turn the light on, if we sense no voltage, turn off
if (digitalRead(Switch1) == HIGH){
 digitalWrite(LED,LOW);
 delay(1000);
 digitalWrite(LED,HIGH);
 delay(100);
 digitalWrite(LED,LOW);
 delay(100);
 digitalWrite(LED,HIGH);
 delay(100);
}
else {
  digitalWrite(LED, LOW); //turn the LED off
 }
}