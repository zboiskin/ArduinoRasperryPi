/*
 * 30 Days - Lost in Space
 * Day 23 - Launch System - creating a count down on the display
 */

/*
 * Arduino concepts introduced/documented in this lesson.
 * - #define macros
 * - rounding up calculated values
 * - elapsed time without using delay() calls.
 * - firstPage()/nextPage() graphics loop to save memory
 */

// Explicitly include Arduino.h
#include "Arduino.h"
#include "Wire.h"

// Include file for 4 digit - 7 segment display library
#include <TM1637Display.h>

/*
 * Extensive documentation for this library can be found at https://github.com/olikraus/u8g2
 * for those wanting to dive deeper, but we will explain all of the functions
 * used in these lessons.
 */
#include <U8g2lib.h>  // Include file for the U8g2 library.

/*
Notes About Macros
#define is useful for text substitutions and can be faster to run than using 'const' since it doesn't need to refer elsewhere
#define CAN cause bugs if the macro is used in multiple places
*/

/*First example of a macro
 * These examples perform a simple calculation returning the minutes or seconds from
 * a number representing milliseconds, rounded UP to the next highest second by adding
 * 999 milliseconds.  This allows our counter to return 00:01 until the instant the
 * counter reaches zero.
*/
#define numberOfMinutes(_milliseconds_) (((_milliseconds_ + 999) / 1000) / 60) //basic structing is we would call the macro plus the input parameter and it would return num of minutes
#define numberOfSeconds(_milliseconds_) (((_milliseconds_ + 999) / 1000) % 60) //numberOfSeconds(1093) and it would perform the calculation specified
//% is the modulo symble and it gives the value of the remainder after a divide 

// Pin connections for our 4 digit counter
#define COUNTER_DISPLAY_CLK_PIN 5
#define COUNTER_DISPLAY_DIO_PIN 4
// Construct counter_display handle.
TM1637Display counter_display(COUNTER_DISPLAY_CLK_PIN, COUNTER_DISPLAY_DIO_PIN);

//OLED library display
U8G2_SH1106_128X64_NONAME_2_HW_I2C lander_display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const byte LANDER_HEIGHT = 25;  // height of our lander image, in bits
const byte LANDER_WIDTH = 20;   // width of our lander image, in bits

// Define 7 Segment display values to spell out "dOnE"
const uint8_t DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

// Define amount of time (in milliseconds) to count down.
const unsigned long COUNTDOWN_MILLISECONDS = 5 * 1000; //set up for 5 seconds

// *********************************************
void setup() {
  Serial.begin(9600);

  // Configure counter display
  counter_display.setBrightness(7);  // Set maximum brightness (value is 0-7)
  counter_display.clear();           // Clear the display

  // Configure OLED display
  lander_display.begin();                     // Initialize OLED display
  lander_display.setFont(u8g2_font_6x10_tr);  // Set text font
  lander_display.setFontRefHeightText();
  lander_display.setFontPosTop(); //makes it so our text starts at the top left 
  
  //this methodology is performed due to limit memory
  //rather than creating and pushing the entire face in the exmaple, we create and push it piece wise
  //function to do this is FirstPage and NextPage, which means defines the parts to push 
  //usually done with WHILE loops  
  lander_display.firstPage();
  do {
	byte y_offset = drawString(0, 0, "Exploration Lander");
    drawString(0, y_offset, "Liftoff Sequence");

    // Status on bottom line of OLED display
    drawString(0, lander_display.getDisplayHeight() - lander_display.getMaxCharHeight(), "Countdown Active");
    // Draw a picture of our lander int bottom right corner
    displayLander(lander_display.getDisplayWidth() - LANDER_WIDTH,
                  lander_display.getDisplayHeight() - LANDER_HEIGHT);
  } while (lander_display.nextPage());

  // blink the countdown on our timer before beginning the countdown
  for (int i = 0; i < 4; i++) {
    counter_display.clear();
    delay(200);
    displayCounter(COUNTDOWN_MILLISECONDS);
    delay(200);
  }
  Serial.println("Countdown started..: ");
}

// *********************************************
  void loop() {
  // Initialize static variables first time through loop() that will retain values
  // between loop() executions.
  static unsigned long timeRemaining = COUNTDOWN_MILLISECONDS;
  static unsigned long countdown_start_time = millis();
  //remember, static values means that the value from the end of the last loop is remembered at the start of the next loop

  Serial.println(timeRemaining);  // Display milliseconds remaining on serial console
  displayCounter(timeRemaining);  // Display minutes:seconds on counter display
  
   // If timeRemaining has reached 0 display ending values
  if (timeRemaining == 0) {
    Serial.println("Done!!");           // indicate completion on serial console
    counter_display.setSegments(DONE);  // "dOnE" on our counter

    // Now update our OLED display with ending screen using firstPage()/nextPage()
    lander_display.firstPage();
    do {
      // Each time we display a line of text on our display the y_offset
      // is updated to point to the next available point for drawing.
      // Display first two lines
      byte y_offset = drawString(0, 0, "Exploration Lander");
      y_offset = drawString(0, y_offset, "Liftoff ABORTED");

      // Set y_offset to point four lines above bottom of display
      y_offset = lander_display.getDisplayHeight() - (4 * lander_display.getMaxCharHeight());
      // Display last four lines
      y_offset = drawString(0, y_offset, "Thrusters: OFF");
      y_offset = drawString(0, y_offset, "Systems: OFF");
      y_offset = drawString(0, y_offset, "Confirm: OFF");
      drawString(0, y_offset, "Countdown ABORT");
      // Draw a picture of our lander in bottom right corner
      displayLander(lander_display.getDisplayWidth() - LANDER_WIDTH,
                    lander_display.getDisplayHeight() - LANDER_HEIGHT);
    } while (lander_display.nextPage());

    // Stop here by looping forever.
    while (1)
      ;
  }

  // Update our remaining time by subtracting the start time from current
  // execution time (in milliseconds).  If our elapsed time is greater
  // than our countdown time then set remaining time to 0.
  unsigned long elapsed_time = millis() - countdown_start_time;
  if (elapsed_time < COUNTDOWN_MILLISECONDS) {
    timeRemaining = COUNTDOWN_MILLISECONDS - elapsed_time;
  } else {
    timeRemaining = 0;
  }
}

// Display milliseconds as minutes:seconds (MM:SS)
void displayCounter(unsigned long milliseconds) {
  // To display the countdown in mm:ss format, separate the parts
  byte minutes = numberOfMinutes(milliseconds);
  byte seconds = numberOfSeconds(milliseconds);

  // Display the minutes in the first two places, with colon
  counter_display.showNumberDecEx(minutes, 0b01000000, true, 2, 0);
  // This displays the seconds in the last two places
  counter_display.showNumberDecEx(seconds, 0, true, 2, 2);
}

// Draw test on our lander display at x, y, returning new y
// value that is immediately below the new line of text.
byte drawString(byte x, byte y, char *string) {
  lander_display.drawStr(x, y, string);
  return (y + lander_display.getMaxCharHeight());  // return new y_offset on display
}

// Draw an image of our lander drawn with frames and triangles
// at location x_location, y_location (relative to the upper left corner).
void displayLander(byte x_location, byte y_location) {
  lander_display.drawFrame(x_location + 7, y_location, 6, 5);        // ship top
  lander_display.drawFrame(x_location + 5, y_location + 4, 10, 20);  // ship center
  lander_display.drawFrame(x_location, y_location + 6, 6, 16);       // left pod
  lander_display.drawFrame(x_location + 14, y_location + 6, 6, 16);  // right pod
  lander_display.drawTriangle(x_location + 2, y_location + 21,
                              x_location, y_location + 25,
                              x_location + 4, y_location + 25);  // left nozzle
  lander_display.drawTriangle(x_location + 18, y_location + 21,
                              x_location + 15, y_location + 25,
                              x_location + 20, y_location + 25);  // right nozzle
}
  
  


