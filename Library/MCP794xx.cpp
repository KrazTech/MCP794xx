/*
Name:		MCP7940.cpp
Created:	5/2/2018 11:39:58 PM
Author:	Chris Krasnichuk
Description:
This file includes all of the function definitions for the MCP7940 library.
The intent is to keep the interaction with the RTC as simple and straight
forwards as possible. Definitions can be found in the header file to simplfy
the use of the library and communication between the RTC.
*/

#include "MCP794xx.h"

////////////////////
// code section
////////////////////

MCP7940::MCP7940() {
	Wire.begin();			// Makes sure the I2C module is operating
}
void MCP7940::start() {

	byte config = readByte(0x00) | 0x80;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to the Second Register
	Wire.write(config);									// Write the "on" bit
	Wire.endTransmission();								// stop transmitting

	while ((readByte(_dateWeekdayReg) & 0x20) == 0);	// Check and wait for the Osc to start running.

	setBattOn();										// Turn the battery on by default

}
void MCP7940::stop() {

	byte config = readByte(0x00) & 0x7F;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(config);									// Write data
	Wire.endTransmission();								// stop transmitting
	while ((readByte(_dateWeekdayReg) & 0x20) != 0);	// Check and wait for the Osc to stop running

}
void MCP7940::setBattOn()
{
	byte buffer = readByte(_dateWeekdayReg) | 0x08;
	writeByte(_dateWeekdayReg, buffer);
}
void MCP7940::setBattOff()
{
	byte buffer = readByte(_dateWeekdayReg) & 0xF7;
	writeByte(_dateWeekdayReg, buffer);
}
void MCP7940::setHours12(int hour, bool _PM)
{
	// Conform Input values
	if (hour > 12) hour = 12;
	if (hour < 1) hour = 1;

	//Errata Fix
	byte config = readByte(0x00) & 0x7F;				// Read Config Data, Clear Run Bit
	writeByte(_timeSecReg, config);						// Write the config data
	while ((readByte(_dateWeekdayReg) & 0x20) != 0);	// Check and wait for the Osc to stop running

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeHourReg);							// Point to Register defined by Address
	Wire.write(decToBcd(hour) | (_PM ? 0x60 : 0x40));	// write the format and 
	Wire.endTransmission();								// stop transmitting

	//Errata Fix
	config = readByte(0x00) | 0x80;						// Read Config Data, Set Run Bit
	writeByte(_timeSecReg, config);						// Write the config data
	while ((readByte(_dateWeekdayReg) & 0x20) == 0);	// Check and wait for the Osc to start running

}
void MCP7940::setHours24(int hour)
{
	// Conform input values
	if (hour == 24 || hour < 0) {
		hour = 0;
	}
	else if (hour > 23) {
		hour = 23;
	}
	
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeHourReg);							// Point to Register defined by Address
	Wire.write(decToBcd(hour) & 0xBF);					// write the Hour.
	Wire.endTransmission();
}
void MCP7940::setMinutes(int minute)
{

	// Conform input values
	if (minute > 59) minute = 59;
	if (minute < 0) minute = 0;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeMinReg);							// Point to Register defined by Address
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setSeconds(int second)
{

	// Conform Input values
	if (second > 59) second = 59;
	if (second < 0) second = 0;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setTime12(int hour, bool _PM, int minute, int second)
{
	// Conform Input values
	if (hour > 12) hour = 12;
	if (hour < 1) hour = 1;
	if (minute > 59) minute = 59;
	if (minute < 0) minute = 0;
	if (second > 59) second = 59;
	if (second < 0) second = 0;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) | (_PM ? 0x60 : 0x40));	// write the format and 
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setTime24(int hour, int minute, int second)
{
	// Conform Input values
	if (hour == 24 || hour < 0) {
		hour = 0;
	}
	else if (hour > 23) {
		hour = 23;
	}
	if (minute > 59) minute = 59;
	if (minute < 0) minute = 0;
	if (second > 59) second = 59;
	if (second < 0) second = 0;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) & 0xBF);					// write the format and 
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setYear(int year)
{
	// Conform input values
	if (year > 99) year = 99;
	if (year < 0) year = 0;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_dateYearReg);							// Point to Register defined by Address
	Wire.write(decToBcd(year));							// Write the year
	Wire.endTransmission();
}
void MCP7940::setMonth(int month)
{
	// Conform input to valid values.
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;

	byte dateBuffer = readByte(_dateDayReg);				// Read Date Register
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateDayReg);								// Point to Register defined by Address
	Wire.write(dateBuffer);									// Rewrite Date register, Errata Fix.
	Wire.write(decToBcd(month));							// Write the month buffer with lear year bit
	Wire.endTransmission();

}
void MCP7940::setDate(int day)
{
	// Conform input values, here best we can do it prevent crazy dates from happening.
	if (day > 31) day = 31;
	if (day < 1) day = 1;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateDayReg);								// Point to Register defined by Address
	Wire.write(decToBcd(day));								// Write the day value
	Wire.endTransmission();									// stop transmitting
}
void MCP7940::setWeekday(int weekday)
{
	// Conform input values
	if (weekday > _SUN) weekday = _SUN;
	if (weekday < _MON) weekday = _MON;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateWeekdayReg);							// Point to Register defined by Address
	Wire.write(decToBcd(weekday) | 0x08);					// Write the weekday value and enabling batt. backup.
	Wire.endTransmission();									// stop transmitting
}
void MCP7940::setCalendar(int year, int month, int day)
{
	// Conform input values
	if (year > 99) year = 99;
	if (year < 0) year = 0;
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;
	if (day > 31) day = 31;
	if (day < 1) day = 1;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateDayReg);								// Point to Register defined by Address
	Wire.write(decToBcd(day));
	Wire.write(decToBcd(month));
	Wire.write(decToBcd(year));
	Wire.endTransmission();
}
int MCP7940::getHours()
{
	byte hourBuff = readByte(_timeHourReg);
	if (hourBuff & 0x40)
	{
		// 12 hour format
		if (hourBuff & 0x20) PM = true;	// Set the PM class variable as true.
		hourBuff &= 0x1F;				// Mask out right bits
	}
	else {
		// 24 hour format
		PM = false;						// Ensure PM is false.
		hourBuff &= 0x3F;				// Mask out the right bits
	}

	return bcdToDec(hourBuff);			// Return the value of the hour
}
int MCP7940::getMinutes()
{
	return bcdToDec(readByte(_timeMinReg));
}
int MCP7940::getSeconds()
{
	return bcdToDec(readByte(_timeSecReg) & 0x7F);
}
int MCP7940::getYear()
{
	byte data[2];
	Wire.beginTransmission(_MCP7940address);	// transmit to RTC
	Wire.write(_dateMonthReg);					// Point to Register defined by Address
	Wire.endTransmission(false);				// Setup for read
	Wire.requestFrom(_MCP7940address, 2);		// Read two bytes
	data[0] = Wire.read();						// Read the month register (contains leapyear data)
	data[1] = Wire.read();						// Read the year register
	Wire.endTransmission();

	if (data[0] & 0x20)							// Check to see if leap year bit is set
	{
		LPYR = true;							// it's a leap year
	}
	else {
		LPYR = false;							// It's not a leap year
	}

	return bcdToDec(data[1]);
}
int MCP7940::getMonth()
{
	return bcdToDec(readByte(_dateMonthReg) & 0x1F);
}
int MCP7940::getDate()
{
	return bcdToDec(readByte(_dateDayReg));
}
int MCP7940::getWeekday()
{
	return bcdToDec(readByte(_dateWeekdayReg) & 0x07);
}
void MCP7940::setAlarmHours12(bool alarmSelect, int hours, bool _PM)
{
	// Conform Input values
	if (hours > 12) hours = 12;
	if (hours < 1) hours = 1;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmHourReg);					// Point to Register defined by Address
	Wire.write(decToBcd(hours) | (_PM ? 0x20 : 0x00));		//  
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	configBuff |= _alarmMatchHours;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//  
	Wire.endTransmission();									// stop transmittin
}
void MCP7940::setAlarmHours24(bool alarmSelect, int hours)
{

	// Conform Input values
	if (hours == 24 || hours < 0) {
		hours = 0;
	}
	else if (hours > 23) {
		hours = 23;
	}

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmHourReg);					// Point to Register defined by Address
	Wire.write(decToBcd(hours));							//  
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	configBuff |= _alarmMatchHours;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//  
	Wire.endTransmission();									// stop transmittin


}
void MCP7940::setAlarmMinutes(bool alarmSelect, int minutes)
{
	// Conform input values
	if (minutes > 59) minutes = 59;
	if (minutes < 0) minutes = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmMinReg);						// Point to Register defined by Address
	Wire.write(decToBcd(minutes));							//  
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	configBuff |= _alarmMatchMinutes;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//  
	Wire.endTransmission();									// stop transmittin
}
void MCP7940::setAlarmSeconds(bool alarmSelect, int seconds)
{
	// Conform input values
	if (seconds > 59) seconds = 59;
	if (seconds < 0) seconds = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmSecReg);						// Point to Register defined by Address
	Wire.write(decToBcd(seconds));							//  
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	configBuff |= _alarmMatchSeconds;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//  
	Wire.endTransmission();									// stop transmittin
}
void MCP7940::setAlarmWeekday(bool alarmSelect, int weekday)
{
	// Conform input values
	if (weekday > _SUN) weekday = _SUN;
	if (weekday < _MON) weekday = _MON;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;			// Select the alarm base register
	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	Wire.beginTransmission(_MCP7940address);						// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);							// Point to Register defined by Address
	Wire.write(configBuff | decToBcd(weekday) | _alarmMatchWeekday);		//  
	Wire.endTransmission();											// stop transmittin
}
void MCP7940::setAlarmDay(bool alarmSelect, int day)
{
	// Conform input values, here best we can do it prevent crazy dates from happening.
	if (day > 31) day = 31;
	if (day < 1) day = 1;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmDateReg);					// Point to Register defined by Address
	Wire.write(decToBcd(day));								//  
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	configBuff |= _alarmMatchDate;

	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//  
	Wire.endTransmission();									// stop transmittin
}
void MCP7940::setAlarmAll12(bool alarmSelect, int month, int date, int hours, bool _PM, int minutes, int seconds)
{
	// Conform Input values
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;
	if (date > 31) date = 31;
	if (date < 1) date = 1;
	if (hours > 12) hours = 12;
	if (hours < 1) hours = 1;
	if (minutes > 59) minutes = 59;
	if (minutes < 0) minutes = 0;
	if (seconds > 59) seconds = 59;
	if (seconds < 0) seconds = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg);									// Point to Register defined by Address
	Wire.write(decToBcd(seconds));
	Wire.write(decToBcd(minutes));
	Wire.write(decToBcd(hours) | (_PM ? 0x20 : 0x00));
	Wire.write(configBuff | _alarmMatchAll);
	Wire.write(decToBcd(date));
	Wire.write(decToBcd(month));
	Wire.endTransmission();									// stop transmittin
}
void MCP7940::setAlarmAll24(bool alarmSelect, int month, int date, int hours, int minutes, int seconds)
{

	// Conform Input values
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;
	if (date > 31) date = 31;
	if (date < 1) date = 1;
	if (hours == 24 || hours < 0) {
		hours = 0;
	}
	else if (hours > 23) {
		hours = 23;
	}
	if (minutes > 59) minutes = 59;
	if (minutes < 0) minutes = 0;
	if (seconds > 59) seconds = 59;
	if (seconds < 0) seconds = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	byte configBuff = readByte(alarmSelect + _alarmWeekdayReg);
	configBuff &= 0x87;
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(baseReg);									// Point to Register defined by Address
	Wire.write(decToBcd(seconds));
	Wire.write(decToBcd(minutes));
	Wire.write(decToBcd(hours));
	Wire.write(configBuff | _alarmMatchAll);
	Wire.write(decToBcd(date));
	Wire.write(decToBcd(month));
	Wire.endTransmission();									// stop transmittin
}
void MCP7940::enableAlarm(bool alarmSelect)
{
	byte configBuff = readByte(_sysCtrlReg);
	configBuff |= alarmSelect ? 0x20 : 0x10;
	writeByte(_sysCtrlReg, configBuff);
}
void MCP7940::disableAlarm(bool alarmSelect)
{
	byte configBuff = readByte(_sysCtrlReg);
	configBuff &= alarmSelect ? 0xDF : 0xEF;
	writeByte(_sysCtrlReg, configBuff);
}
byte MCP7940::checkAlarm(bool alarmSelect)
{
	byte ctrlReg = (readByte(_sysCtrlReg) & 0x30);			// Read Control reg and mask out alarm en. bits.
	byte regBuff;
	byte minBuff;
	byte flags = 0x00;

	switch (ctrlReg)
	{
	case 0x30:
		// Alarm 1 & 2 are enabled
	case 0x10:
		// Alarm 0 is enabled

		regBuff = readByte(_alarm0WeekdayReg);				// Read the weekday reg, has all the info we're interested in
		if (regBuff & 0x08)									// Check the alarm IF flag
		{
			switch (regBuff & 0x70)							// Mask off the alarm match mask
			{
			case 0x00:
				// Match on Second
				flags |= _0MatchSec;						// Set flags

				break;
			case 0x10:
				// Match on minutes

				// Double check for minute match as per Errata.
				minBuff = readByte(_timeMinReg);			// Read Minute Register
				regBuff = readByte(_alarm0MinReg);			// Read Alarm Minute Register

				if (minBuff == regBuff) flags |= _0MatchMin;	// Set flags

				break;
			case 0x20:
				// Match on hours
				flags |= _0MatchHours;						// Set flags

				break;
			case 0x30:
				// Match on Day of the week
				flags |= _0MatchWeekday;					// Set flags

				break;
			case 0x40:
				// Match on the day of the month
				flags |= _0MatchDate;						// Set flags

				break;
			case 0x70:
				// Match on Seconds, Minutes, Hours, Day of the Week, Date, and Month.
				flags |= _0MatchAll;						// Set flags

			default:
				break;
			}
		}

		if (ctrlReg == 0x30) goto alarm1;					// If both alarms are set, skip the break

		break;
	case 0x20:
	alarm1:
		// Alarm 1 is enabled

		regBuff = readByte(_alarm1WeekdayReg);				// Read the weekday reg, has all the info we're interested in
		if (regBuff & 0x08)									// Check the alarm IF flag
		{
			switch (regBuff & 0x70)							// Mask off the alarm match mask
			{
			case 0x00:
				// Match on Second
				flags |= _1MatchSec;						// Set flags

				break;
			case 0x10:
				// Match on minutes

				// Double check for minute match as per Errata.
				minBuff = readByte(_timeMinReg);			// Read Minute Register
				regBuff = readByte(_alarm1MinReg);			// Read Alarm Minute Register

				if (minBuff == regBuff) flags |= _1MatchMin;	// Set flags

				break;
			case 0x20:
				// Match on hours
				flags |= _1MatchHours;						// Set flags

				break;
			case 0x30:
				// Match on Day of the week
				flags |= _1MatchWeekday;					// Set flags

				break;
			case 0x40:
				// Match on the day of the month
				flags |= _1MatchDate;						// Set flags

				break;
			case 0x70:
				// Match on Seconds, Minutes, Hours, Day of the Week, Date, and Month.
				flags |= _1MatchAll;						// Set flags
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}
int MCP7940::getPwrDownHours()
{
	byte hourBuff = readByte(_pwrDnHourReg);
	if (hourBuff & 0x40)
	{
		// 12 hour format
		if (hourBuff & 0x20) PM = true;	// Set the PM class variable as true.
		hourBuff &= 0x1F;				// Mask out right bits
	}
	else {
		// 24 hour format
		hourBuff &= 0x3F;				// Mask out the right bits
	}

	return bcdToDec(hourBuff);			// Return the value of the hour
}
int MCP7940::getPwrDownMinutes()
{
	return bcdToDec(readByte(_pwrDnMinReg));
}
int MCP7940::getPwrDownMonth()
{
	return bcdToDec(readByte(_pwrDnMonthReg) & 0x1F);
}
int MCP7940::getPwrDownDate()
{
	return bcdToDec(readByte(_pwrDnDateReg));
}
int MCP7940::getPwrDownWeekday()
{
	return bcdToDec((readByte(_pwrDnMonthReg) & 0xE0) >> 5);
}
int MCP7940::getPwrUpHours()
{
	byte hourBuff = readByte(_pwrUpHourReg);
	if (hourBuff & 0x40)
	{
		// 12 hour format
		if (hourBuff & 0x20) PM = true;	// Set the PM class variable as true.
		hourBuff &= 0x1F;				// Mask out right bits
	}
	else {
		// 24 hour format
		hourBuff &= 0x3F;				// Mask out the right bits
	}

	return bcdToDec(hourBuff);			// Return the value of the hour
}
int MCP7940::getPwrUpMinutes()
{
	return bcdToDec(readByte(_pwrUpMinReg));
}
int MCP7940::getPwrUpMonth()
{
	return bcdToDec(readByte(_pwrUpMonthReg) & 0x1F);
}
int MCP7940::getPwrUpDate()
{
	return bcdToDec(readByte(_pwrUpDateReg));
}
int MCP7940::getPwrUpWeekday()
{
	return bcdToDec((readByte(_pwrUpMonthReg) & 0xE0) >> 5);
}
void MCP7940::setMFPin(bool value)
{
	byte configBuff = readByte(_sysCtrlReg);		// Read current configuration
	configBuff &= 0x0F;
	writeByte(_sysCtrlReg, configBuff | (value ? 0x80 : 0x00));

}
void MCP7940::setMFPinSquareWave(int selectOut)
{
	byte configBuff = readByte(_sysCtrlReg);		// Read current configuration
	configBuff &= 0xBC;

	switch (selectOut) {
	case _1Hz:

		// 1 Hz
		configBuff |= 0x40;
		break;

	case _4kHz:

		// 4.096 kHz
		configBuff |= 0x41;
		break;

	case _8kHz:

		// 8.192 kHz
		configBuff |= 0x42;
		break;

	case _32kHz:

		// 32.768 kHz
		configBuff |= 0x43;
		break;

	default:
		// Turns off the squarewave output
		break;
	}

	writeByte(_sysCtrlReg, configBuff);
}
void MCP7940::writeData(byte reg, byte* buffer, int numBytes)
{
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(reg);
	for (int i = 0; i < numBytes;i++)
	{
		Wire.write(buffer[i]);
		if ((reg + i) == _dat0x3F) break;				// Prevent an overflow condition 
	}
	Wire.endTransmission();								// stop transmitting
}
byte MCP7940::readData(byte reg, byte* buffer, int numBytes)
{
	Wire.beginTransmission(_MCP7940address);	// transmit to RTC
	Wire.write(reg);							// Point to starting Register
	Wire.endTransmission(false);

	if (numBytes > 32)
	{
		// Arduino's I2C buffer is only 32 bytes.
		// to get around this we perform the large read in segments.
		for (int j = 0; j < (numBytes / 32); j++)
		{
			Wire.requestFrom(_MCP7940address, numBytes);
			for (int i = 0; i < numBytes; i++)
			{
				buffer[(j * 32) + i] = Wire.read();
				if ((reg + (j * 32) + i) >= _dat0x3F) break;				// Prevent an overflow condition 
			}
		}
	}
	else {
		Wire.requestFrom(_MCP7940address, numBytes);
		for (int i = 0; i < numBytes;i++)
		{
			buffer[i] = Wire.read();
			if ((reg + i) >= _dat0x3F) break;				// Prevent an overflow condition 
		}
	}
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::standbyMode()
{
	// May be left unimplemented
}

/////////////////////
// Private Functions
/////////////////////

void MCP7940::writeByte(byte address, byte data) {
	Wire.beginTransmission(_MCP7940address);   // transmit to RTC
	Wire.write(address);            // Point to Register defined by Address
	Wire.write(data);               // Write data
	Wire.endTransmission();         // stop transmitting
	return;
}
byte MCP7940::readByte(byte address) {
	byte data;

	Wire.beginTransmission(_MCP7940address);   // transmit to RTC
	Wire.write(address);            // Point to Register defined by Address
	Wire.endTransmission(false);

	Wire.requestFrom(_MCP7940address, 1);

	data = Wire.read();            // Read the register

	Wire.endTransmission();

	return data;
}
byte MCP7940::bcdToDec(byte bcd)
{

	return(((bcd >> 4) * 10) + (bcd & 0xF));
}
byte MCP7940::decToBcd(byte val)
{
	return(((val / 10) << 4) | (val % 10));
}

