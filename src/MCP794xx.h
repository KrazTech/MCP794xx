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

/// MCP794xx Parameter Definitions
//! \def MCP794xx I2C Address
#define _MCP794xxaddress		0x6F

//! \enum registers Defines all the MCP794xx registers for easier access
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

/// Alarm Base Registers
#define _alarm0Reg			0x0A
#define _alarm1Reg			0x11

/// Alarm Register OFFSETs
enum alarmOffsets {
	_alarmSecReg,
	_alarmMinReg,
	_alarmHourReg,
	_alarmWeekdayReg,
	_alarmDateReg,
	_alarmMonthReg
};

/// Alarm Register Mask Definitions
#define _alarm1Bit			0x20
#define _alarm0Bit			0x10
#define _24h				0x00
#define _12h				0x40
#define _timeFormatBit		0x40
#define _alarmIFBit			0x08

/// Alarm Configuration Definitions *Note, These are configured for an active high alarm output
#define _alarmMatchSeconds	0x00
#define _alarmMatchMinutes	0x10
#define _alarmMatchHours	0x20
#define _alarmMatchWeekday	0x30
#define _alarmMatchDate		0x40
#define _alarmMatchAll		0x70

/*  Alarm Status Definitions
 	bitwise flags:
 	|ALM1IF|3 BITS FOR ALM1 MATCH CONFIG|ALM0IF|3 BITS FOR ALM0 MATCH CONFIG|
*/

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



//! \enum months Enumerates the months in a year for easier use
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

//! \enum weekdays Enumerates the days of the week for easier & consistent use
enum weekdays {
	_MON=0x01,
	_TUE,
	_WED,
	_THUR,
	_FRI,
	_SAT,
	_SUN
};

//! \enum sqWaveConfig Square Wave Output Frequency
enum sqWaveConfig {
	_32kHz,	///< Configures Squarewave output at ~32 kHz
	_8kHz,	///< Configures Squarewave output at ~8 kHz
	_4kHz,	///< Configures Squarewave output at ~4 kHz
	_1Hz	///< Configures Squarewave output at ~1 kHz
};

/** \class MCP794xx 
	\brief Container for the MCP794xx functions
	This class is used to contain all the interacting functions
	within an object.
*/
class MCP794xx {
public:

	// Flags
	bool PM = NULL;			///< Signifies if the last getHours() call had a 12h result in the PM.
	bool LPYR = NULL;		///< Signifies if the last getYear() resulted in a leap year.

	MCP794xx();

	void start();													///< Activate the RTC Clock
	void stop();													///< Stop the RTC Clock
	void setBattOn();												///< Turn the battery backup ON (on by default)
	void setBattOff();												///< Turn the battery backup OFF
	/**
	\param hour the hour to be set, in 12 hour format.
	\param _PM indicates if the hour to be set is AM (false) or PM (true)
	*/
	void setHours12(int hour, bool _PM);							///< Set the hour in 12 Hour format
	/**
	\param hour the hour to be set, in 24 hour format.
	*/
	void setHours24(int hour);										///< Set the hour in 24 Hour format
	/**
	\param minute the minute to be set. 
	*/
	void setMinutes(int minute);									///< Set the minutes
	/**
	\param second the second to be set. 
	*/
	void setSeconds(int second);									///< Set the seconds
	/**
	\param hour the hour to be set, in 12 hour format.
	\param _PM indicates if the hour to be set in AM (false) or PM (true)
	\param minute the minute to be set.
	\param second the second to be set.
	*/
	void setTime12(int hour, bool _PM, int minute, int second);		///< Set the time in 12 Hour format
	/**
	\param hour the hour to be set, in 24 hour format.
	\param minute the minute to be set.
	\param second the second to be set.
	*/
	void setTime24(int hour, int minute, int second);				///< Set the time in 24 Hour format
	/**
	\param year the last two digits of the year to be set.
	*/
	void setYear(int year);											///< Set the year
	/**
	\param month the month to be set.
	*/
	void setMonth(int month);										///< Set the month
	/**
	\param date the day of the month to be set
	*/
	void setDate(int date);											///< Set the day of the month
	/**
	\param weekday the day of the week to be set. Values from 1-7 (or use the enumeration)
	*/
	void setWeekday(int weekday);									///< Set the weekday (Monday, Tuesday, ...)
	/**
	\param year the last two digits of the year to be set.
	\param month the month to be set
	\param date the day of the month to be set
	*/
	void setCalendar(int year, int month, int date);				///< Set the Date (Weekday must be set seperately)
	/** Returns the hour (returns hour, for 12 hour format check PM variable)
	 * \return the hour
	 */
	int getHours();	
	/** Returns the minute of the hour
	 * \return the minute
	 */
	int getMinutes();
	/** Returns the seconds of the minute
	 * \return seconds
	 */
	int getSeconds();
	/** Returns the year (last 2 digits)
	 * \return the year
	 */
	int getYear();
	/** Returns the month
	 * \return the month
	 */
	int getMonth();
	/** Returns the day of the month
	 * \return the day of the month
	 */
	int getDate();	

	/** Returns the weekday
	 * \return the day of the week
	 */
	int getWeekday();

	/**
	\param alarmSelect used to select which alarm to set
	\param hours the hour which triggers the alarm (12h format)
	\param _PM PM indicator for the hours, should be TRUE if the hour is in the PM
	*/
	void setAlarmHours12(bool alarmSelect, int hours, bool _PM);	///< Set alarm 0/1 to trigger on a match of hours 12 hour format
	/**
	\param alarmSelect used to select which alarm to set
	\param hours the hour which triggers the alarm (24h format)
	*/
	void setAlarmHours24(bool alarmSelect, int hours);				///< Set alarm 0/1 to trigger on a match of hours 12 hour format
	/**
	\param alarmSelect used to select which alarm to set
	\param minutes the minute which triggers the alarm
	*/
	void setAlarmMinutes(bool alarmSelect, int minutes);			///< Set alarm 0/1 to trigger on a match of minutes
	/**
	\param alarmSelect used to select which alarm to set
	\param seconds the second which triggers the alarm
	*/
	void setAlarmSeconds(bool alarmSelect, int seconds);			///< Set alarm 0/1 to trigger on a match of seconds
	/**
	\param alarmSelect used to select which alarm to set
	\param weekday the day of the week which triggers the alarm. Values from 1-7 (or use the enumeration)
	*/
	void setAlarmWeekday(bool alarmSelect, int weekday);			///< Set alarm 0/1 to trigger on a match of weekday
	/**
	\param alarmSelect used to select which alarm to set
	\param date the day of the month which triggers the alarm (1-31)
	*/
	void setAlarmDate(bool alarmSelect, int date);					///< Set alarm 0/1 to trigger on a match of day of the month
	/**
	\param alarmSelect used to select which alarm to set
	\param month the month which must match for alarm to trigger
	\param date the day of the motnh which must match for the alarm to trigger
	\param weekday the day of the week which must match for the alarm to trigger
	\param hours the hour which must match for the alarm to trigger (12h format)
	\param _PM indicates whether the hour is in the AM or PM (TRUE if in PM)
	\param minutes the minute which must match for the alarm to trigger
	\param seconds the second which must match for the alarm to trigger
	*/
	void setAlarmAll12(bool alarmSelect, int month, int date, int weekday, int hours, bool _PM, int minutes, int seconds);		///< Set alarm 0/1 to trigger on a match of seconds, minutes, hours, weekday, day, and month
	/**
	\param alarmSelect used to select which alarm to set
	\param month the month which must match for alarm to trigger
	\param date the day of the motnh which must match for the alarm to trigger
	\param weekday the day of the week which must match for the alarm to trigger
	\param hours the hour which must match for the alarm to trigger (24h format)
	\param minutes the minute which must match for the alarm to trigger
	\param seconds the second which must match for the alarm to trigger
	*/
	void setAlarmAll24(bool alarmSelect, int month, int date, int weekday, int hours, int minutes, int seconds);   				///< Set alarm 0/1 to trigger on a match of seconds, minutes, hours, weekday, day, and month
	/**
	\param alarmSelect used to select which alarm to enable
	*/					
	void enableAlarm(bool alarmSelect);								///< Enable alarm 0/1
	/**
	\param alarmSelect used to select which alarm's interrupt flag to clear
	*/
	void clearFlag(bool alarmSelect);								///< Clears Alarm Interrupt Flag
	/**
	\param alarmSelect used to select which alarm to disable
	*/
	void disableAlarm(bool alarmSelect);							///< Disable alarm 0/1
	/** \fn checkAlarm
		\brief Returns a bitwise flag byte indicating which alarm went off and why.

		The returned byte is formatted as follows:

 		|ALM1IF|3 BITS FOR ALM1 MATCH CONFIG|ALM0IF|3 BITS FOR ALM0 MATCH CONFIG|

		performing a bitwise AND (&) operation using the following masks:
				
		_0statusIF
		_0statusMask
		_0matchSec
		_0matchMin
		_0matchHours
		_0matchWeekday	
		_0matchDate			
		_0matchAll			
		_1statusIF			
		_1statusMask		
		_1matchSec			
		_1matchMin			
		_1matchHours		
		_1matchWeekday	
		_1matchDate			
		_1matchAll	

		Will indicate which alarm went off and the match conditions.

		\return formatted byte containing flags indicating which alarm went off and why

	*/
	byte checkAlarm();												

	/** \fn getPwrDownHours
	 	Returns the hour when the Vin power was cut-off
		Works for both 12/24h formats
		12h format uses the _PM class variable to indicate if the hour is in the PM

		\return the hour when power was cut-off to the board
	*/
	int getPwrDownHours();
	/** Returns the minute when the Vin power was cut-off
	 * \return the minute when when power was cut-off to the board 
	 */										
	int getPwrDownMinutes();
	/** Returns the month when the Vin power was cut-off
	 * \return the month when the pwoer was cut-off to the board
	 */
	int getPwrDownMonth();
	/** Returns the day of the month when Vin power was cut-off
	 * \return the day of the month when power was cut-off to the baord
	 */
	int getPwrDownDate(); 
	/** Returns the day of the week (1-7) when Vin power was cut-off
	 * \return the day of the week when power was cut-off to the board
	 */
	int getPwrDownWeekday(); 

	/** \fn getPwrUpHours
	 	Returns the hour when the Vin power was applied
		Works for both 12/24h formats
		12h format uses the _PM class variable to indicate if the hour is in the PM
		\return the hour when power was applied to the board
	*/
	int getPwrUpHours();
	/** Returns the minute when the Vin power was applied 
	 * \return the minute when power was applied to the board
	 */											
	int getPwrUpMinutes();
	/** Returns the month when the Vin power was applied
	 * \return the month when power was applied to the board
	 */
	int getPwrUpMonth();
	/** Returns the day of the month when Vin power was applied
	 * \return the day of the month when power was applied to the board
	 */
	int getPwrUpDate();	
	/** Returns the day of the week (1-7) when Vin power was applied
	 * \return the day of the week when power was applied to the board
	 */
	int getPwrUpWeekday();

	/** Sets the value of the Multifunction pin, disables alarms
	 * \param value the boolean value (0/1) to apply to the MFPin.
	 */
	void setMFPin(bool value);

	/** Configures the Multifunction pin to output a square wave, disables alarms
	 * \param selectOut configuration parameter to set the output frequency. See enumeration.
	 */
	void setMFPinSquareWave(int selectOut);

	/** write a stream of data to the RTC module's SRAM
	 * \param reg the register address to begin writing at
	 * \param buffer pointer to the first buffer element
	 * \param numBytes number of bytes to write to the device
	 */
	void writeData(byte reg, byte* buffer, int numBytes);
	/** read a stream of data from the RTC module's SRAM
	 * \param reg the register address to begin reading at
	 * \param buffer pointer to the array which will store the read bytes
	 * \param numBytes number of bytes to read from the device
	 */
	void readData(byte reg, byte* buffer, int numBytes);
	/** currently unused
	 */
	void standbyMode();

private:
	byte bcdToDec(byte bcd);
	byte decToBcd(byte val);

	void writeByte(byte address, byte data);
	byte readByte(byte address);


};

#endif

