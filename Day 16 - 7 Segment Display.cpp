//Day 16 - Working with 7 Segment Display
/* Key Concepts and Functions
 //7 Segment Display - Digital clocks are usually made up of 7 segment displays make up of 7 LED segments A-G
 //Telling which segment to turn on and off takes binary input 1=ON, 0=OFF for the 7 segments A-G
 //Set.Segments function – like the puppet master for our 7 segment display and tells which segment
 //Binary notation (0b00000001)
 */
 
 //Always included
 #include "Arduino.h"
 
 //New library to use functions to operate the 7 segment display
 #include <TM1637Display.h>
 
 /*
 * A 7-segment display is shaped like an "8" and has 7 segments (A through G) that can be
 * lit up. The diagram below shows the placement of each of the segments.
 *
 *   A
 * F   B
 *   G
 * E   C
 *   D
 */
 
 //Define which pins we are using the 7 segment display
 const byte CLK_PIN = 6; //clock pin
 const byte DIO_PIN = 5; //digital input/output
 
 //Creating a display model, similar to how we mapped out how our keypad is going to look
 TM1637Display hero_display = TM1637Display(CLK_PIN, DIO_PIN);
 
 // BONUS SHORTHAND
 // Create array that turns all segments on:
const byte all_on[] = { 0b11111111,
                        0b11111111,
                        0b11111111,
                        0b11111111 };
/*
//This is a way to use binary to turn on and off the segments
//In this case, each 1/0 represents one of the 7 segments A-G
//In the above, all 7 segments are set to 1 meaning turn them all on
//Remember, there are 4 display blocks each with its own 7 segments, so we need 4 rows with the binary
//Further example 0b11111111 means turn segment A,B,C,D,E,F,G all on but if it was 0b11111110, then turn on A,B,C,D,E,F on but leave G off
*/
 
 //Additional Example using more explicit long form that is easier to read, similar to above but without binary
 //Here we are making the word "DONE"
 const byte done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d - turn segment B,C,D,E,G on - leave the others not mentioned, off
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

void setup() {
	hero_display.setBrightness(7); //this just sets how bright exact we was the display segment to be when we do actually turn it on 
}

//NOTE - remember, we do not need to explicitly set all display segments as outputs just like we did not need to set all 16 keys in our keypad as outputs
//This is all handled by our library

void loop() {
	//This part clears the display to start from scratch and turns all lights off
	hero_display.clear();
	delay(1000);
	
	//This part lights up all segments in the display
	hero_display.setSegments(all_on);
	
	//And back to clearing just to show that all the wiring is working correctly
	hero_display.clear();
	delay(1000);
	
//This is another example of how to input show to 12:00
//below is saying run through this loop 4 times, starting at 0 and adding 1 for each run though (int i is the counter)
//another function included in the library is showNumberDecEx - this allows us to just show the time we want without doing the binary or manual segmenting above
//0b01000000 input parameter is just binary for a colon ':'
  for (int i = 0; i < 4; i++) {
    hero_display.showNumberDecEx(1200, 0b01000000);
    delay(500);
    hero_display.clear();
    delay(500);
  }
  
//Another example to show more functionality - it can also display - numbers
//below is saying to run the loop through from -100 to 100 starting at -100 and count up to -99, -98, -97, etc.
//once complete, jump out of the loop, clear the display and display our DONE variable we made above
  for (int i = -100; i <= 100; i++) {
    hero_display.showNumberDec(i);
    delay(50);
  }
  delay(1000);

  // Clear the display (all segments off)
  hero_display.clear();
  delay(1000);

  // Display the message "dOnE"
  hero_display.setSegments(done);

  delay(10000);  // Delay 10 seconds and then repeat our demo.
}