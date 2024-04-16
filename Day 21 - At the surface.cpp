/*
 * 30 Days - Lost in Space
 * Day 21 - Hello New World!
 */

/*
 * Arduino concepts introduced/documented in this lesson.
 * - I2C communications.
 * - Computer character fonts
 * - U8g2 graphics library for monochrome displays
 * - Logical Not operator ('!')
 * - clearBuffer()/sendBuffer() method for OLED display updates
 *
 * Parts and electronics concepts introduced in this lesson.
 * - SH1106 monochrome 128x64 pixel OLED display.
 * - HERO I2C pins (A5 and A4)
 */

// Explicitly include Arduino.h
#include "Arduino.h"

// Include file for the U8g2 library to display graphics and messages on our display
#include <U8g2lib.h>  

//This is our specific type of display we are using for today pulled from the above new library
//we will call this variable 'lander display'
U8G2_SH1106_128X64_NONAME_F_HW_I2C lander_display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

//In setup we are telling our output pin A4 to begin showing us our display on the hardware
void setup() {
	Serial.begin(9600);
	delay(1000);
	lander_display.begin();
	
//Setting our font with .setFront
  lander_display.setFont(u8g2_font_ncenB08_tr);
}


void loop() {
//Have to make sure to set the font height to ensure that if we change the font, it won't go over the size of our display screen
	byte font_height = lander_display.getMaxCharHeight();
	lander_display.clearBuffer(); //clear display and internal memory
	lander_display.setFontPosTop(); //ensures that the text display will automatically start at the top coordinate position
	drawCenteredString(0, "Exploration Lander"); //finds the center of the device we are using to display
	drawCenteredString(font_height, "Hello World!"); //display our message on line 2 - this will stay static on our screen at all times 
	
	//to create a blinking message, we make a static bool blink on = true 
	//every time it goes through the loop this value changes - so first time it runs it will be true or on, then at the end of the loop it will be set to false and turn off_type
	//it will keep looping through and changing from true to false and back to true after each loop, making the message blink on when true, off when false
	static bool blink_on = true;
	if (blink_on) {
	/*
	this below part finds the horizontal and vertical center and stores it as a variable 
	so that we can display our messages center of the screen
	we do this rather than hard code the center, because if we want to change the font, we can automatically find the center no matter the fint
	*/
		byte centered_y = (font_height * 2) + ((lander_display.getDisplayHeight() - (font_height * 2)) / 2);
	//once all of the centering and font positioning is set up we can begin with our message
		lander_display.setFontPosCenter();
		drawCenteredString(centered_y, "Stand by");
	}
	
	blink_on = !blink_on; //same thing as = true ---this is what has the blink_on change from true to false back to true every time we loop through 
	
	// Nothing is displayed on our display until the buffer is sent to the display, below.
    lander_display.sendBuffer();  // transfer internal memory to the display
    delay(500);   // Delay for blink effect
}

    // Use the .drawStr() method to draw the current string centered in
    // the current display.
    byte drawCenteredString(byte y, char *string) {
    byte centered_x = (lander_display.getDisplayWidth() - lander_display.getStrWidth(string)) / 2;
    lander_display.drawStr(centered_x, y, string);
}
	
