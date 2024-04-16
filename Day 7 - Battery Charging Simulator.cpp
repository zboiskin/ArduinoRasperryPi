//Today's code will be used to simulate charging our battery

//using this analog pin, we still read the current like level
const byte PHOTORESISTOR_PIN = A0;

// unsighed as in neither + or -, setting to what will act as our max or full battery charge or capacity
const unsigned int BATTERY_CAPACITY = 50000;

void setup() {
  pinMode(PHOTORESISTOR_PIN, INPUT); //setting our pin called PHOTORESISTOR assigned to A0 pin as our input variable

  Serial.begin(9600); //setting our serial monitor to 9600 so we can actually understand the reading we get from our monitor
}

unsigned int battery_level = 0; //simulating our current battery charge of 0

void loop() {
  
 //display battery info on the serial monitor
  if (battery_level < BATTERY_CAPACITY) { //if battery is not fully charged  
    battery_level += analogRead(PHOTORESISTOR_PIN); //read light level and add to battery level
      if (battery_level > BATTERY_CAPACITY) {         //if this would make charge over 100%
        battery_level = BATTERY_CAPACITY;           // then set current level to max capacity
      }
 }

 printBatteryChargeLevel(); //display current charge percentage on Serial Monitor

 delay(100);
}

void printBatteryChargeLevel() {
  if (battery_level < BATTERY_CAPACITY) {  // if not fully charged
    // Percentage of charge is current level divided by capacity, multiplied by 100 to get a percentage.
    Serial.print(((double)battery_level / (double)BATTERY_CAPACITY) * 100);  // display charge % to Serial Monitor
    Serial.println("%");
  } else {
    Serial.println("FULLY CHARGED");  // ...indicate fully charged on Serial Monitor
  }
}