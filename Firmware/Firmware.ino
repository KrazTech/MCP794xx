/*
Name:		ArduinoExample.ino
Created:	5/2/2018 11:42:00 PM
Author:	Chris Krasnichuk
Description:

To test the functionality of the Arduino library for the MCP7940 RTC Module.

*/

// the setup function runs once when you press reset or power the board

#include <Wire.h>
#include <MCP794xx.h>

MCP794xx rtc1;

void setup() {

	// Start Serial Communication
	Serial.begin(115200);

	// Run through setting the date and time.
	rtc1.setHours12(11, false); // Set the time to 11 AM
	rtc1.setMinutes(39);		// Set the minutes to 39
	rtc1.setSeconds(45);		// Set the second to 45
	rtc1.setYear(18);			// Set the year to 18
	rtc1.setMonth(_FEB);		// Set the Month
	rtc1.setDate(23);			// Set the date (0-31)
	rtc1.setWeekday(_THUR);		// Set the day of the week
	rtc1.start();				// Turn on the clock, begins tracking time


	// Read back the date and time.
	Serial.print("Time: ");
	Serial.print(rtc1.getHours());		// returns Hours in int format
	Serial.print("h ");
	Serial.print(rtc1.getMinutes());	// returns minutes in int format
	Serial.print("m ");
	Serial.print(rtc1.getSeconds());	// returns seconds in in format.
	Serial.print(" s");

	// The PM value of the MCP7940 class is updated every time the hours are read from the module.
	// PM is TRUE if the time is in 12 hour format and in the after noon (i.e. 3:45 PM)
	// Otherwise PM is false
	rtc1.PM ? Serial.print(" PM") : Serial.print(" AM");
	Serial.print("\n");

	Serial.print("Weekday: ");

	// Returns the Day of the Week value, these can be user defined but the library contains predefined values
	Serial.print(rtc1.getWeekday());
	Serial.print(" | Date: ");
	Serial.print(rtc1.getDate());		// Returns the date of the month.
	Serial.print(" | Month: ");
	Serial.print(rtc1.getMonth());		// Returns the month
	Serial.print(" | Year: ");
	Serial.print(rtc1.getYear());		// Returns the last two digits of the year
	Serial.print("\n");

	rtc1.setCalendar(55, _NOV, 1);		// Set the calendar with one function call

	delay(1000);

	Serial.print("Weekday: ");
	Serial.print(rtc1.getWeekday());
	Serial.print(" | Date: ");
	Serial.print(rtc1.getDate());
	Serial.print(" | Month: ");
	Serial.print(rtc1.getMonth());
	Serial.print(" | Year: ");
	Serial.print(rtc1.getYear());
	Serial.print("\n");

	// Set the Multifunction pin to output a square wave.

	rtc1.setMFPinSquareWave(_32kHz);
	delay(3000);
	rtc1.setMFPinSquareWave(_8kHz);
	delay(3000);
	rtc1.setMFPinSquareWave(_4kHz);
	delay(3000);
	rtc1.setMFPinSquareWave(_1Hz);
	delay(3000);


}

// the loop function runs over and over again until power down or reset
void loop() {


	Serial.print("Time: ");
	Serial.print(rtc1.getHours());
	Serial.print("h ");
	Serial.print(rtc1.getMinutes());
	Serial.print("m ");
	Serial.print(rtc1.getSeconds());
	Serial.print(" s");
	rtc1.PM ? Serial.print(" PM") : Serial.print(" AM");
	Serial.print("\n");

	rtc1.setMFPinSquareWave(_32kHz);
	delay(2000);
	rtc1.setMFPinSquareWave(_8kHz);
	delay(2000);
	rtc1.setMFPinSquareWave(_4kHz);
	delay(2000);
	rtc1.setMFPinSquareWave(_1Hz);
	delay(2000);
}
