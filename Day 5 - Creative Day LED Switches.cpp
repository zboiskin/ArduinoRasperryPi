//day 5 idea - 
//harder - day 4 wiring but extend it to 6 lights and rather than the 3 switches on/off, 
int LED1 = 13;
int LED2 = 12;
int LED3 = 11;
int LED4 = 10;
int LED5 = 9;
int LED6 = 8;
int Switch1 = 2;
int Switch2 = 3;
int Switch3 = 4;

void setup() {
pinMode(LED1, OUTPUT); 
pinMode(LED2, OUTPUT); 
pinMode(LED3, OUTPUT); 
pinMode(LED4, OUTPUT); 
pinMode(LED5, OUTPUT); 
pinMode(LED6, OUTPUT); 
pinMode(Switch1, INPUT);
pinMode(Switch2, INPUT);
pinMode(Switch3, INPUT);
}

void loop() {
	//LED 1
	if (digitalRead(Switch1) == HIGH){
		digitalWrite(LED1, HIGH);
	}
	else {digitalWrite(LED1,LOW);
	}
	//LED 2
	if (digitalRead(Switch2) == HIGH){
	digitalWrite(LED2, HIGH);
	}
	else {digitalWrite(LED2,LOW);
	}
	//LED 3
	if (digitalRead(Switch3) == HIGH){
	digitalWrite(LED3, HIGH);
	}
	else {digitalWrite(LED3,LOW);
	}
	//LED 4
	if (digitalRead(Switch1) == HIGH
	&& digitalRead(Switch2) == HIGH){
	digitalWrite(LED4, HIGH);
	}
	else {digitalWrite(LED4,LOW);
	}
	//LED 5
	if (digitalRead(Switch1) == HIGH
	&& digitalRead(Switch3) == HIGH){
	digitalWrite(LED5, HIGH);
	}
	else {digitalWrite(LED5,LOW);
	}
	//LED 6
	if (digitalRead(Switch2) == HIGH
	&& digitalRead(Switch3) == HIGH){
	digitalWrite(LED6, HIGH);
	}
	else {digitalWrite(LED6,LOW);
	}
}
