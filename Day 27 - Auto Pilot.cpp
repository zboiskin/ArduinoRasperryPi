/*
 * 30 Days - Lost in Space
 * Day 27 - Fun With Bitmaps, Part 2
 *
 *
 * In addition, we won't just be displaying all the bitmaps in a loop so
 * we need a way to *select* which bitmaps are being displayed.
 *
 * Let's use our DIP switches to display one of eight different bitmaps.
 * Since we only have three switches we'll read each as a bit in a binary
 * number.  This way three switches can give us eight distinct values.
 *
 * We'll also add our 4 digit 7 segment display to show which picture
 * is currently being displayed.
 *
 */

/*
 * Arduino concepts introduced/documented in this lesson.
 * - partial bitmaps
 * - switches to binary
 * - creating include files
 *
 * Parts and electronics concepts introduced in this lesson.
 * - 
 */

// Explicitly include Arduino.h
#include "Arduino.h"

// Extensive documentation for this library can be found at https://github.com/olikraus/u8g2
#include <U8g2lib.h>  // Include file for the U8g2 library.
#include "Wire.h"     // Sometimes required for I2C communications.

// Use _2_ version of constructor and firstPage()/nextPage() for OLED
// graphics to save memory.
U8G2_SH1106_128X64_NONAME_2_HW_I2C lander_display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

/*
In addition to forward declarations like we did yesterday by assigning a name to a bit map in the beginning and then referring to it in the loop
We can do something even simpler by creating a header file with all the bitmaps we need in another tab and then simply #include it - make sure tab is the same name
 */
#include "switch_bitmaps.h"

// Create an array of pointers to each of the bitmap images.
const static char* SWITCH_BITMAPS[] = {
  SWITCHES_ZERO,
  SWITCHES_ONE,
  SWITCHES_TWO,
  SWITCHES_THREE,
  SWITCHES_FOUR,
  SWITCHES_FIVE,
  SWITCHES_SIX,
  SWITCHES_SEVEN,
};

// Include file for 4 digit - 7 segment display library
#include <TM1637Display.h>
const byte BITMAP_NUMBER_DISPLAY_DIO_PIN = 2;
const byte BITMAP_NUMBER_DISPLAY_CLK_PIN = 3;
// Construct bitmap_number_display handle.
TM1637Display bitmap_number_display(BITMAP_NUMBER_DISPLAY_CLK_PIN, BITMAP_NUMBER_DISPLAY_DIO_PIN);

// Define pins for our DIP switches
// We will need almost all pins for Day 29, so here we show that we can use ANALOG
// pins as digitial pins when necessary.
const byte SWITCH_BIT_0_PIN = A2;  // switch for bit 0 of our 3 bit value
const byte SWITCH_BIT_1_PIN = A1;  // switch for bit 1 of our 3 bit value
const byte SWITCH_BIT_2_PIN = A0;  // switch for bit 2 of our 3 bit value

// ************************************************
void setup(void) {
  Serial.begin(9600);

  // Configure counter display
  bitmap_number_display.setBrightness(7);  // Set maximum brightness (value is 0-7)
  bitmap_number_display.clear();           // Clear the display

  // Configure DIP switch pins
  pinMode(SWITCH_BIT_0_PIN, INPUT);  // switch for bit 0 of our 3 bit value
  pinMode(SWITCH_BIT_1_PIN, INPUT);  // switch for bit 1 of our 3 bit value
  pinMode(SWITCH_BIT_2_PIN, INPUT);  // switch for bit 2 of our 3 bit value
  //analog pins are backwards compatable to be used as digital pins 
  //BUT digital pins cannot be made to be analog pins


  lander_display.begin();  // initialize lander display
}

// ************************************************
void loop(void) {
  // Calculate our x and y offsets for our bitmap graphics
  byte x_offset = (lander_display.getDisplayWidth() - BITMAP_WIDTH) / 2;
  byte y_offset = (lander_display.getDisplayHeight() - BITMAP_HEIGHT) / 2;

  /*
   * Now we build up a 3 bit binary number by reading each of our switches,
   * ensuring that the read value is 1 for on and 0 for off (using the
   * ternary notation), and then shifting each bit to it's desired location.
   * This converts our three switches to the values 0-7:
   *   0b00000000 = 0
   *   0b00000001 = 1
   *   0b00000010 = 2
   *   0b00000011 = 3
   *   0b00000100 = 4
   *   0b00000101 = 5
   *   0b00000110 = 6
   *   0b00000011 = 7
   */

  // Read bit 0 (0b00000001), ensure 0 or 1 and save.
  byte switch_value = digitalRead(SWITCH_BIT_0_PIN) == HIGH ? 1 : 0;
  // Read bit 1 (0b00000010), ensure 0 or 1, shift left 1 bit and OR it into current value.
  switch_value |= (digitalRead(SWITCH_BIT_1_PIN) == HIGH ? 1 : 0) << 1;
  // Read bit 2 (0b00000100), ensure 0 or 1, shift left 2 bits and OR it into current value.
  switch_value |= (digitalRead(SWITCH_BIT_2_PIN) == HIGH ? 1 : 0) << 2;

  // Display calculated switch value on our 4 digit display
  bitmap_number_display.showNumberDecEx(switch_value);

  // Display selected bitmap on our OLED lander display
  lander_display.firstPage();
  do {
    // .drawXBMP() displays each bitmap centered in the display based
    // on it's size.
    lander_display.drawXBMP(x_offset, y_offset, BITMAP_WIDTH, BITMAP_HEIGHT, SWITCH_BITMAPS[switch_value]);
  } while (lander_display.nextPage());

  delay(100);
}


