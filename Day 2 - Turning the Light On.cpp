int light = 12 ; //HERO board pin 12

void setup() {
// initialize a digital pin as an output, then set its value to HIGH (5 volts)
pinMode(light, OUTPUT);
digitalWrite(light, HIGH);
}

void loop() {
//turns the light on for half a second and off for one second
digitalWrite(light, LOW);
delay(1000);
digitalWrite(light, HIGH);
delay(500);
}
