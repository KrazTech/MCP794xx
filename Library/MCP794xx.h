/*
Name:		MCP794xx.h
Created:	5/2/2018 11:39:58 PM
Author:	Chris Krasnichuk

*/

#include <Wire.h>
#include <HardwareSerial.h>

#ifndef _MCP794xx_h
#define _MCP794xx_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

///////////////////
// Definitions
///////////////////

// MCP794xx Parameter Definitions
#define _MCP794xxaddress		0x6F

// Register Definitations
enum registers {
	_timeSecReg,
	_timeMinReg,
	_timeHourReg,
	_dateWeekdayReg,
	_dateDateReg,
	_dateMonthReg,
	_dateYearReg,
	_sysCtrlReg,
	_sysOscTrimReg,
	_reserved0,
	_alarm0SecReg,
	_alarm0MinReg,
	_alarm0HourReg,
	_alarm0WeekdayReg,
	_alarm0DateReg,
	_alarm0MonthReg,
	_reserved1,
	_alarm1SecReg,
	_alarm1MinReg,
	_alarm1HourReg,
	_alarm1WeekdayReg,
	_alarm1DateReg,
	_alarm1MonthReg,
	_reserved2,
	_pwrDnMinReg,
	_pwrDnHourReg,
	_pwrDnDateReg,
	_pwrDnMonthReg,
	_pwrUpMinReg,
	_pwrUpHourReg,
	_pwrUpDateReg,
	_pwrUpMonthReg,
	_dat0x00,
	_dat0x01,
	_dat0x02,
	_dat0x03,
	_dat0x04,
	_dat0x05,
	_dat0x06,
	_dat0x07,
	_dat0x08,
	_dat0x09,
	_dat0x0A,
	_dat0x0B,
	_dat0x0C,
	_dat0x0D,
	_dat0x0E,
	_dat0x0F,
	_dat0x10,
	_dat0x11,
	_dat0x12,
	_dat0x13,
	_dat0x14,
	_dat0x15,
	_dat0x16,
	_dat0x17,
	_dat0x18,
	_dat0x19,
	_dat0x1A,
	_dat0x1B,
	_dat0x1C,
	_dat0x1D,
	_dat0x1E,
	_dat0x1F,
	_dat0x20,
	_dat0x21,
	_dat0x22,
	_dat0x23,
	_dat0x24,
	_dat0x25,
	_dat0x26,
	_dat0x27,
	_dat0x28,
	_dat0x29,
	_dat0x2A,
	_dat0x2B,
	_dat0x2C,
	_dat0x2D,
	_dat0x2E,
	_dat0x2F,
	_dat0x30,
	_dat0x31,
	_dat0x32,
	_dat0x33,
	_dat0x34,
	_dat0x35,
	_dat0x36,
	_dat0x37,
	_dat0x38,
	_dat0x39,
	_dat0x3A,
	_dat0x3B,
	_dat0x3C,
	_dat0x3D,
	_dat0x3E,
	_dat0x3F
};

// Alarm Base Registers
#define _alarm0Reg			0x0A
#define _alarm1Reg			0x11

// Alarm Register OFFSETs
enum alarmOffsets {
	_alarmSecReg,
	_alarmMinReg,
	_alarmHourReg,
	_alarmWeekdayReg,
	_alarmDateReg,
	_alarmMonthReg
};

// Alarm Register Mask Definitions
#define _alarm1Bit			0x20
#define _alarm0Bit			0x10
#define _24h				0x00
#define _12h				0x40
#define _timeFormatBit		0x40
#define _alarmIFBit			0x08

// Alarm Configuration Definitions *Note, These are configured for an active high alarm output
#define _alarmMatchSeconds	0x00
#define _alarmMatchMinutes	0x10
#define _alarmMatchHours	0x20
#define _alarmMatchWeekday	0x30
#define _alarmMatchDate		0x40
#define _alarmMatchAll		0x70

// Alarm Status Definitions
// bitwise flags:
// |ALM1IF|3 BITS FOR ALM1 MATCH CONFIG|ALM0IF|3 BITS FOR ALM0 MATCH CONFIG|

#define _0statusIF			0x08
#define _0statusMask		0x0F
#define	_0matchSec			0x08
#define	_0matchMin			0x09
#define	_0matchHours		0x0A
#define	_0matchWeekday		0x0B
#define	_0matchDate			0x0C
#define	_0matchAll			0x0F
#define _1statusIF			0x80
#define _1statusMask		0xF0
#define	_1matchSec			0x80
#define	_1matchMin			0x90
#define	_1matchHours		0xA0
#define	_1matchWeekday		0xB0
#define	_1matchDate			0xC0
#define	_1matchAll			0xF0



// Day/Month Definitions
enum months {
	_JAN = 0x01,
	_FEB,
	_MAR,
	_APR,
	_MAY,
	_JUN,
	_JUL,
	_AUG,
	_SEPT,
	_OCT,
	_NOV,
	_DEC
};

enum weekdays {
	_MON=0x01,
	_TUE,
	_WED,
	_THUR,
	_FRI,
	_SAT,
	_SUN
};

// SquareWave Configuration definitions
enum sqWaveConfig {
	_32kHz,
	_8kHz,
	_4kHz,
	_1Hz
};


class MCP794xx {
public:

	// Flags
	bool PM = NULL;			// Signifies if the last getHours() call had a 12h result in the PM.
	bool LPYR = NULL;		// Signifies if the last getYear() resulted in a leap year.

	MCP794xx();

	void start();													// Activate the RTC Clock
	void stop();													// Stop the RTC Clock
	void setBattOn();
	void setBattOff();
	void setHours12(int hour, bool _PM);							// Set the hour in 12 Hour format
	void setHours24(int hour);										// Set the hour in 24 Hour format
	void setMinutes(int minute);									// Set the minutes
	void setSeconds(int second);									// Set the seconds
	void setTime12(int hour, bool _PM, int minute, int second);		// Set the time in 12 Hour format
	void setTime24(int hour, int minute, int second);				// Set the time in 24 Hour format
	void setYear(int year);											// Set the year
	void setMonth(int month);										// Set the month
	void setDate(int date);											// Set the day of the month
	void setWeekday(int weekday);									// Set the weekday (Monday, Tuesday, ...)
	void setCalendar(int year, int month, int date);					// Set the Date (Weekday must be set seperately)
	int getHours();													// Returns the hour (returns hour, for 12 hour format check PM variable)
	int getMinutes();												// Returns the minute of the hour
	int getSeconds();												// Returns the seconds of the minute
	int getYear();													// Returns the year (Last 2 digits)
	int getMonth();													// Returns the month
	int getDate();													// Returns the day of the month
	int getWeekday();												// Returns the weekday

	void setAlarmHours12(bool alarmSelect, int hours, bool _PM);	// Set alarm 0/1 to trigger on a match of hours 12 hour format
	void setAlarmHours24(bool alarmSelect, int hours);				// Set alarm 0/1 to trigger on a match of hours 12 hour format
	void setAlarmMinutes(bool alarmSelect, int minutes);			// Set alarm 0/1 to trigger on a match of minutes
	void setAlarmSeconds(bool alarmSelect, int seconds);			// Set alarm 0/1 to trigger on a match of seconds
	void setAlarmWeekday(bool alarmSelect, int weekday);			// Set alarm 0/1 to trigger on a match of weekday
	void setAlarmDate(bool alarmSelect, int date);					// Set alarm 0/1 to trigger on a match of day of the month
	void setAlarmAll12(bool alarmSelect, int month, int date, int weekday, int hours, bool _PM, int minutes, int seconds);		// Set alarm 0/1 to trigger on a match of seconds, minutes, hours, weekday, day, and month
	void setAlarmAll24(bool alarmSelect, int month, int date, int weekday, int hours, int minutes, int seconds);					// Set alarm 0/1 to trigger on a match of seconds, minutes, hours, weekday, day, and month
	void enableAlarm(bool alarmSelect);								// Enable alarm 0/1
	void clearFlag(bool alarmSelect);
	void disableAlarm(bool alarmSelect);							// Disable alarm 0/1
	byte checkAlarm();												// Check which alarm went off, and what triggered it, returns status value (check alarmStatus enum)

	int getPwrDownHours();
	int getPwrDownMinutes();
	int getPwrDownMonth();
	int getPwrDownDate();
	int getPwrDownWeekday();

	int getPwrUpHours();
	int getPwrUpMinutes();
	int getPwrUpMonth();
	int getPwrUpDate();
	int getPwrUpWeekday();

	void setMFPin(bool value);										// Sets the value of the Multifunction pin, disables alarms
	void setMFPinSquareWave(int selectOut);							// Configures the Multifunction pin to output a sqaure wave, disables alarms

	void writeData(byte reg, byte* buffer, int numBytes);												// Write a byte of data to SRAM
	byte readData(byte reg, byte* buffer, int numBytes);												// read a byte of data from SRAM
	void standbyMode();

private:
	byte bcdToDec(byte bcd);
	byte decToBcd(byte val);

	void writeByte(byte address, byte data);
	byte readByte(byte address);


};

#endif

