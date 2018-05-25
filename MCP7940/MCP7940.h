/*
 Name:		MCP7940.h
 Created:	5/2/2018 11:39:58 PM
 Author:	Chris Krasnichuk

*/

#include <Wire.h>
#include <HardwareSerial.h>

#ifndef _MCP7940_h
#define _MCP7940_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///////////////////
// Definitions
///////////////////

// MCP7940 Parameter Definitions
#define _MCP7940address		0x6F

// Register Definitations
#define _timeSecReg			0x00
#define _timeMinReg			0x01
#define _timeHourReg		0x02
#define _dateWeekdayReg		0x03
#define _dateDayReg			0x04
#define _dateMonthReg		0x05
#define _dateYearReg		0x06
#define _sysCtrlReg			0x07
#define _sysOscTrimReg		0x08


/*
enum registers {
	_timeSecReg = 0x00,
	_timeMinReg,
	_timeHourReg,
	_dateWeekdayReg,
	_dateDayReg,
	_dateMonthReg,
	_dateYearReg,
};*/

// Alarm STUFF***
#define _alarm0Reg			0x0A
#define _alarm1Reg			0x11

// Alarm Register OFFSETs
#define _alarmSecReg		0x00
#define _alarmMinReg		0x01
#define _alarmHourReg		0x02
#define _alarmWeekdayReg	0x03
#define _alarmDateReg		0x04
#define _alarmMonthReg		0x05

// Register Mask Definitions
#define _alarm1Bit			0x20
#define _alarm0Bit			0x10
#define _24h				0x00
#define _12h				0x40
#define _timeFormatBit		0x40
#define _alarmIFBit			0x08

// Alarm Configuration Definitions
#define _alarmMatchSeconds	0x00
#define _alarmMatchMinutes	0x10
#define _alarmMatchHours	0x20
#define _alarmMatchWeekday	0x30
#define _alarmMatchDate		0x40
#define _alarmMatchAll		0x70

// Day/Month Definitions
#define JAN				0x01
#define FEB				0x02
#define MAR				0x03
#define APR				0x04
#define MAY				0x05
#define JUN				0x06
#define JUL				0x07
#define AUG				0x08
#define SEPT			0x09
#define OCT				0x10
#define NOV				0x11
#define DEC				0x12

#define _MON				0x01
#define _TUE				0x02
#define _WED				0x03
#define _THUR				0x04
#define _FRI				0x05
#define _SAT				0x06
#define _SUN				0x07


class MCP7940 {
public:
	MCP7940();

	void start();													// Activate the RTC Clock
	void stop();													// Stop the RTC Clock
	void setHours12(int hour, bool _PM);							// Set the hour in 12 Hour format
	void setHours24(int hour);										// Set the hour in 24 Hour format
	void setMinutes(int minute);									// Set the minutes
	void setSeconds(int second);									// Set the seconds
	void setTime12(int hour, bool _PM, int minute, int second);		// Set the time in 12 Hour format
	void setTime24(int hour, int minute, int second);				// Set the time in 24 Hour format
	void setYear(int year, bool leapYear);							// Set the year
	void setMonth(int month);										// Set the month
	void setDay(int day);											// Set the day of the month
	void setWeekday(int weekday);									// Set the weekday (Monday, Tuesday, ...)
	void setDate(int year, bool leapYear, int month, int day);		// Set the Date (Weekday must be set seperately)
	int getHours();													// Returns the hour (returns hour, for 12 hour format check PM variable)
	int getMinutes();												// Returns the minute of the hour
	int getSeconds();												// Returns the seconds of the minute
	int getYear();													// Returns the year (Last 2 digits)
	int getMonth();													// Returns the month
	int getDay();													// Returns the day of the month
	int getWeekday();												// Returns the weekday
		
	void setAlarmHours12(bool alarmSelect, int hours, bool _PM);	// Set alarm 0/1 to trigger on a match of hours 12 hour format
	void setAlarmHours24(bool alarmSelect, int hours);				// Set alarm 0/1 to trigger on a match of hours 12 hour format
	void setAlarmMinutes(bool alarmSelect, int minutes);			// Set alarm 0/1 to trigger on a match of minutes
	void setAlarmSeconds(bool alarmSelect, int seconds);			// Set alarm 0/1 to trigger on a match of seconds
	void setAlarmWeekday(bool alarmSelect, int weekday);			// Set alarm 0/1 to trigger on a match of weekday
	void setAlarmDay(bool alarmSelect, int day);					// Set alarm 0/1 to trigger on a match of day of the month
	void setAlarmAll12(bool alarmSelect, int month, int date, int hours, bool _PM, int minutes, int seconds);		// Set alarm 0/1 to trigger on a match of seconds, minutes, hours, weekday, day, and month
	void setAlarmAll24(bool alarmSelect, int month, int date, int hours, int minutes, int seconds);		// Set alarm 0/1 to trigger on a match of seconds, minutes, hours, weekday, day, and month
	void enableAlarm(bool alarmSelect);								// Enable alarm 0/1
	void disableAlarm(bool alarmSelect);							// Disable alarm 0/1

	int getPwrDownHours();
	int getPwrDownMinutes();
	int getPwrDownSeconds();
	int getPwrDownMonth();
	int getPwrDownDay();

	int getPwrUpHours();
	int getPwrUpMinutes();
	int getPwrUpSeconds();
	int getPwrUpMonth();
	int getPwrUpDay();

	void setMFPin(bool value);										// Sets the value of the Multifunction pin, disables alarms
	void setMFPinSquareWave(byte config);							// Configures the Multifunction pin to output a sqaure wave, disables alarms

	void attachInterrupt();											// Configures an interrupt routine to a pin for the alarm
	void detachInterrupt();											// detaches an terrupt routine
	void writeData();												// Write a byte of data to SRAM
	byte readData();												// read a byte of data from SRAM
	void standbyMode();

	void setTime(byte format, int hour, int minute, int second);
	void setDate(bool leapYear, int year, int month, int day, int weekday);
	void readTime();
	void readDate();


	void setAlarmAll(byte alarm, int month, int day, int weekday, int hour, int minute, int second, byte PM);
	void setAlarmWeekday(byte alarm, int weekday);
	void setAlarmDate(byte alarm, int day);
	void setAlarmHour(byte alarm, int hour, byte PM);
	void setAlarmMinute(byte alarm, int minute);
	void setAlarmSecond(byte alarm, int second);

	void disableAlarm(byte alarm);


	// Variables
	bool PM = NULL;
	bool LPYR = NULL;


private:
	byte bcdToDec(byte bcd);
	byte decToBcd(byte val);

	void writeByte(byte address, byte data);
	byte readByte(byte address);


};

#endif

