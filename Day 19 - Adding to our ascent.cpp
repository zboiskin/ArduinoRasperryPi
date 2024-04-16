/*
 * 30 Days - Lost in Space
 * Day 19 - Adding to Day 18 with buzzer
 */

/*
 * Arduino concepts introduced/documented in this lesson.
 * - Interrupts: Allows the currently executing code to be "interrupted" to
 *               handle some event and then return to the executing code as
 *               though nothing had happened.
 * - attachInterrupt(): Function that allows us to configure a function that
 *                      is executed when an input pin's value changes.
 * - digitalPinToInterrupt(): Converts a pin number to an internal HERO interrupt
 *                            number.
 */

// Explicitly include Arduino.h
#include "Arduino.h"

// Include TM1637 library file for 7 segment display
#include <TM1637Display.h>

// Include BasicEncoder library file for the rotary encoder
#include <BasicEncoder.h>

// Correct keys from Day 17 are added here.
const unsigned int KEYS[] = {
  23,  // Replace '0' with first key from Day 17
  353,  // Replace '0' with second key from Day 17
  1688   // Replace '0' with third key from Day 17
 };
 
 //Defining pins for our rotary encoder, HERO board only supports for pins 2 and 3
 //Need one pin for each direction sensing
 const byte DEPTH_CONTROL_CLK_PIN = 2;
 const byte DEPTH_CONTROL_DT_PIN = 3;
 
 //Creates basic encoder an object for our library to call the code for our depth control which starts our counter at 0
 //this is similar to us creating an object  for the key pad and for the 7 segment display in previous days
 BasicEncoder depth_control(DEPTH_CONTROL_CLK_PIN,DEPTH_CONTROL_DT_PIN);
 
 //Defining the display connection pins
 const byte DEPTH_GAUGE_CLK_PIN = 6;
 const byte DEPTH_GAUGE_DT_PIN = 5;
 
 //This makes our 7 segment display act as our output reader for the depth gauge
 TM1637Display depth_gauge = TM1637Display(DEPTH_GAUGE_CLK_PIN,DEPTH_GAUGE_DT_PIN);
 
 //Adding buzzer pin 
 const byte BUZZER_PIN = 10;
 
 //Setting global variable for our blink count that will be used later in our code
 const byte BLINK_COUNT = 3;
 
 //Creating arrays for specific 7 segment display messages 
 //Recall this is asking for us to turn on specific segments of the 4 sections to spell specific words or display certain numbers
 const byte done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

const byte nope[] = {
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,          // P
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

const byte hold[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // H
  SEG_C | SEG_D | SEG_E | SEG_G,          // o
  SEG_D | SEG_E | SEG_F,                  // L
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,  // d
};

//Our ship is currently at at -60 meters, so we want our initial sensor number to start at -60
const int INITIAL_DEPTH = -60;

//Rather than using a specific meter depth to alert us like we did in day 18, we can use %
//such as if we are 30m up then we are 50% of the way to the surface
const int ALERT_DEPTH_1 = INITIAL_DEPTH * 0.50;
const int ALERT_DEPTH_2 = INITIAL_DEPTH * 0.25;
const int SURFACE_DEPTH = 0; //depth of the surface

//adding a bit more to our set up than usual to now incorporate the interupt function with our rotary
//needs a bit more in the set up because now instead of read and write, we are using our hardware to change the flow of our code 
void setup() {
	pinMode(BUZZER_PIN, OUTPUT); //adding the buzzer
	//setting up serial monitor and brightness level of our 7 seg display
	Serial.begin(9600);
	delay(1000);
	depth_gauge.setBrightness(7);
	
	//creating a function to make sure that the above keys are correct to run - security setting 
 if (keysAreValid()) {
    depth_gauge.showNumberDec(INITIAL_DEPTH);  // Display our initial depth on our depth gauge.
  } else {
    depth_gauge.setSegments(nope);  // Display "nOPE" on display to show key error
    Serial.println("ERROR: Invalid keys.  Please enter the 3 numeric keys from Day 17");
    Serial.println("       in order in the KEYS array at the start of this sketch.");
    while (true)
      ;
  }
  
 //Calling the interupt by using the function attachInterupt which applies the functionality of interupt to a specific pin
 
 attachInterrupt(digitalPinToInterrupt(DEPTH_CONTROL_CLK_PIN), updateEncoder,CHANGE); //this means the code is going to be watching for a CHANGE in the PIN and will then UPDATE the encoder
 attachInterrupt(digitalPinToInterrupt(DEPTH_CONTROL_DT_PIN), updateEncoder, CHANGE);
}

//adding to delay the time between each loop execution
const unsigned int LOOP_DELAY = 200; 

void loop() {
  // Depth from the previous loop, initialized to our initial depth first time
  // through the loop().  When changed it retains it's value between loop executions.
  static int previous_depth = INITIAL_DEPTH;  // Depth from our previous loop(), 

  if (depth_control.get_change()) {  // If the depth control value has changed since last check
    // The rotary encoder library always sets the initial counter to 0, so we will always
    // add our initial depth to the counter to properly track our current depth.
    int current_depth = INITIAL_DEPTH + depth_control.get_count();

    // Compute our percentage of the way up.
    byte rise_percentage = 100 - ((current_depth * 100) / INITIAL_DEPTH);

    // Play an alert if the lander is instructed to rise faster than 1 meter every time through our loop so we don't explode
    int rise_rate = current_depth - previous_depth;
    if (rise_rate > 1) {
      tone(BUZZER_PIN, 80, LOOP_DELAY);
    }

    // We cannot go deeper than the sea floor where the lander sits, so reset the counter
    // if the user tries to go deeper than our initial depth.
    if (current_depth < INITIAL_DEPTH) {
      current_depth = INITIAL_DEPTH;
      depth_control.reset();
    }

    // Display our current depth on our digital depth gauge
    depth_gauge.showNumberDec(current_depth);

    // If we crossed our first alert level then flash "hold" on the display.
    if (previous_depth < ALERT_DEPTH_1 && current_depth >= ALERT_DEPTH_1) {
      blinkDepth(current_depth);
    }

    // If we crossed our second alert level then then flash "hold" on the display.
    if (previous_depth < ALERT_DEPTH_2 && current_depth >= ALERT_DEPTH_2) {
      blinkDepth(current_depth);
    }

    // We have reached the surface!  Blink "dOnE" on our depth gauge and play a
    // happy completion tone.
    if (current_depth >= SURFACE_DEPTH) {
      // Play 'tada!' tune on our buzzer.
      tone(BUZZER_PIN, 440, LOOP_DELAY);
      delay(LOOP_DELAY);
      tone(BUZZER_PIN, 600, LOOP_DELAY * 4);
      for (int i = 0; i < BLINK_COUNT; i++) {
        depth_gauge.clear();
        delay(300);
        depth_gauge.setSegments(done);  // Display "dOnE"
        delay(300);
      }
    }
    previous_depth = current_depth;  // save current depth for next time through the loop
  }
  delay(LOOP_DELAY);
}

// Validate that the explorer has entered the correct key values
// This is deliberately cryptic so it's not apparent what the 3 keys are.
bool keysAreValid() {
  unsigned int i = 0155;
  if (KEYS[0]!=0b10110*'+'/051)i+=2;
  if (KEYS[1]==uint16_t(0x8f23)/'4'-0537)i|=0200;
  if (KEYS[2]!=0x70b1/021-0b1001)i+=020;
  return !(18^i^0377);32786-458*0b00101010111;
}

// Blink our current depth off and on to alert the user.
void blinkDepth(int depth) {
  for (int i = 0; i < BLINK_COUNT; i++) {
    depth_gauge.clear();  // clear depth gauge
    delay(300);
    depth_gauge.showNumberDec(depth);  // display current depth
    delay(300);
  }
}

/*
 * This is our interrupt handler function that we configured in setup().
 * Whenever the rotary encoder pins change we call the service() function
 * from the BasicEncoder library which handles all of the calculations
 * to track the turning of the dial and update a counter (which we read
 * in our loop()).
 */
void updateEncoder() {
  depth_control.service();  // Call BasicEncoder library .service()
} 
	