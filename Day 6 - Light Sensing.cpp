/*
//Functions Used:
 * - unsigned int: A 16 bit value containing numbers from 0 to 65535
 * - Serial Monitor: Allows the HERO to display text in an Arduino IDE window.
 * - Serial.begin(): Used to initialize the Serial Monitor.
 * - Serial.print(): Display some text in the Arduino IDE Serial Monitor window.
 * - Serial.println(): Display test in the Serial Monitor followed by a newline.
 * - analogRead(): Read a value from an analog pin that is based on how much voltage is on the pin (0-5v)
 */

// A0 is a label specifically for analog reading
// Our photoresistor will connect to this and give us a reading of the current light level 
const byte PHOTORESISTOR_PIN = A0;

// These two constants set the minimum and maximum or range of ms for delay times for our blinking LED
// The type "unsigned int" represents numbers from 0 to 65535.  
// We need this since these delay values can be greater than the biggest number a byte can represent (255).
const unsigned int MIN_DELAY = 50;   // 50 ms shortest blink delay
const unsigned int MAX_DELAY = 500;  // 500 ms longest blink delay

// One time setup
void setup() {
  // We will blink our build in LED based on amount of light received from our photoresistor
  pinMode(LED_BUILTIN, OUTPUT);       // output since we will control the built in LED
  pinMode(PHOTORESISTOR_PIN, INPUT);  // input value from analog pin connected to photoresistor

  /*
   * To show you the exact value being read on the analog pin we will print the exact number using our Arduino IDE's "Serial Monitor".
   * The speed that this data is sent/received must match between the Arduino IDE and HERO.
   * We configure this speed for the HERO to send data using the Serial.begin() function. using 9600 for 9600 bits of info per second
   */
  Serial.begin(9600);
}

// The loop() function is called over and over when sketch is run.
void loop() {
  /*
   * Each time through our loop() we will read the current value of our photoresistor.  When
   * the voltage goes up from more light we flash the built-in LED faster (with shorter blinks).
   *
   * These pins will convert a voltage from 0V to 5V to a number from 0 to 1023, giving us
   * a full range of values.  Because we are reading an analog value instead of a digital
   * value, we use the analogRead() function.
   *
   * Here we use the reading from the PHOTORESISTOR_PIN and modify how long we delay based on it.
   */
  unsigned int light_value = analogRead(PHOTORESISTOR_PIN);   // light value from 0 to 1024
  Serial.print("Light value: ");  // Display label string to serial monitor
  Serial.print(light_value);      // display the value read from our photoresistor

  /*
   * The flash rate varies based on the relative brightness received by the photoresistor. So we add the below static values to keep the 
   * specific room's brightest and dimmest light to set the range when this is run
   *
   * These values would normally be lost when each run of our loop() ends, but by adding
   * the "static" declaration we indicate that these local variables should maintain their values between loop() runs.
   */
  static unsigned int darkest_light = light_value;    // this is the lowest value returned by the photoresistor
  static unsigned int brightest_light = light_value;  // this is the highest value returned by the photoresistor

  /*
   * Below keeps the value updating when it senses a bright or dim light. Only when it senses a brighter light than currently saved, that new
   * value becomes the new top limit 
   */
  if (light_value < darkest_light) {  // If value is LESS THAN ('<') the darkest...
    darkest_light = light_value;      // ...then save current value as our new darkest value
  }
  if (light_value > brightest_light) {  // If value is GREATER THAN ('>') the brightest...
    brightest_light = light_value;      // ...then save current value as our new brightest value
  }

  /*
   * Now we have an interesting problem.  As our voltage reading goes up, we want the LED to
   * blink faster (and thus with a smaller delay).  In addition, we need to adjust the blink
   * rate in a noticeable way.  So, we have one range of numbers (darkest to brightest values)
   * from our light sensor that we wish to convert to a different range that goes from MAX_DELAY
   * down to MIN_DELAY.
   *
   * function named map() will essentially normalize our ratings so that even though our real range is 0-6000, 
   *we map it to 0-100
   * this properly matches our brightness/dimness level to our delay between blink times
   */
  unsigned int delay_value = map(light_value, darkest_light, brightest_light, MAX_DELAY, MIN_DELAY);
  Serial.print(", Delay value: ");  // display label after light_value
  Serial.println(delay_value);      // display delay_value returned by map() function with newline

  // now blink our built in LED using our delay_value.
  digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED
  delay(delay_value);               // Leave on delay_value milliseconds
  digitalWrite(LED_BUILTIN, LOW);   // Turn off LED
  delay(delay_value);               // Leave off delay_value milliseconds
}