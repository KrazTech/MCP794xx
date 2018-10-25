/*
Name:		Firmware.ino
Created:	2018-10-14
Author:	Chris Krasnichuk
Description:

Example code for the Kraztech MCP794xx RTC Module

*/

#include <Wire.h>
#include <MCP794xx.h>


MCP794xx rtc1;

void setup() {

	// Start Serial Communication
	Serial.begin(115200);

	// Run through setting the date and time.
	rtc1.setHours12(11, false); // Set the time to 11 AM
	//rtc1.setHours24(11);		// Set the time to 1100 (24h format)
	rtc1.setMinutes(39);		// Set the minutes to 39
	rtc1.setSeconds(45);		// Set the second to 45
	
	// You can also set the time in a single function call
	rtc1.setTime12(11, false, 39, 45);			// Set the time in 12h format
	//rtc1.setTime24(11, 39, 45);				// Set the time in 24h format

	
	
	rtc1.setYear(18);			// Set the year to 18
	rtc1.setMonth(_FEB);		// Set the Month
	rtc1.setDate(23);			// Set the date (0-31)
	rtc1.setWeekday(_THUR);		// Set the day of the week

	// You can also set the date in a single function call
	rtc1.setCalendar(18, _FEB, 23);	// Set the calendar, weekday must be set seperately
	rtc1.setWeekday(_THURS);		// Set the weekday.

	rtc1.start();				// Turn on the clock, begins tracking time

	delay(1000);

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
	if (rtc1.PM == true)
	{
		Serial.print(" PM\n");
	} else {
		Serial.print(" AM\n");
	}

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
