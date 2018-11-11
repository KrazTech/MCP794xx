/**
 * @file MCP794xx.cpp
 *
 * @mainpage Kraztech MCP794xx RTC Module Driver
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for the Kraztech's MCP794xx driver for the Arduino platform.
 * The goal of the driver is to keep it as simple and straight forward as possible
 * so that beginners can easily and quickly pickup the board and begin having fun
 * with all the time tracking possibilities!
 *
 * @section author Author
 *
 * Written by Chris Krasnichuk at Kraztech
 *
 *
*/

#include "MCP794xx.h"

////////////////////
// code section
////////////////////

MCP794xx::MCP794xx() {
	Wire.begin();			// Makes sure the I2C module is operating
}
void MCP794xx::start() {

	byte config = readByte(0x00) | 0x80;

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to the Second Register
	Wire.write(config);									// Write the "on" bit
	Wire.endTransmission();								// stop transmitting

	while ((readByte(_dateWeekdayReg) & 0x20) == 0);	// Check and wait for the Osc to start running.

	setBattOn();										// Turn the battery on by default

}
void MCP794xx::stop() {

	byte config = readByte(0x00) & 0x7F;

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(config);									// Write data
	Wire.endTransmission();								// stop transmitting
	while ((readByte(_dateWeekdayReg) & 0x20) != 0);	// Check and wait for the Osc to stop running

}
void MCP794xx::setBattOn()
{
	byte buffer = readByte(_dateWeekdayReg) | 0x08;
	writeByte(_dateWeekdayReg, buffer);
}
void MCP794xx::setBattOff()
{
	byte buffer = readByte(_dateWeekdayReg) & 0xF7;
	writeByte(_dateWeekdayReg, buffer);
}
void MCP794xx::setHours12(int hour, bool _PM)
{
	// Conform Input values
	if (hour > 12) hour = 12;
	if (hour < 1) hour = 1;

	//Errata Fix
	byte config = readByte(0x00) & 0x7F;				// Read Config Data, Clear Run Bit
	writeByte(_timeSecReg, config);						// Write the config data
	while ((readByte(_dateWeekdayReg) & 0x20) != 0);	// Check and wait for the Osc to stop running

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeHourReg);							// Point to Register defined by Address
	Wire.write(decToBcd(hour) | (_PM ? 0x60 : 0x40));	// write the format and
	Wire.endTransmission();								// stop transmitting

	//Errata Fix
	config = readByte(0x00) | 0x80;						// Read Config Data, Set Run Bit
	writeByte(_timeSecReg, config);						// Write the config data
	while ((readByte(_dateWeekdayReg) & 0x20) == 0);	// Check and wait for the Osc to start running

}
void MCP794xx::setHours24(int hour)
{
	// Conform input values
	if (hour == 24 || hour < 0) {
		hour = 0;
	}
	else if (hour > 23) {
		hour = 23;
	}

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeHourReg);							// Point to Register defined by Address
	Wire.write(decToBcd(hour) & 0xBF);					// write the Hour.
	Wire.endTransmission();
}
void MCP794xx::setMinutes(int minute)
{

	// Conform input values
	if (minute > 59) minute = 59;
	if (minute < 0) minute = 0;

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeMinReg);							// Point to Register defined by Address
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.endTransmission();								// stop transmitting
}
void MCP794xx::setSeconds(int second)
{

	// Conform Input values
	if (second > 59) second = 59;
	if (second < 0) second = 0;

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.endTransmission();								// stop transmitting
}
void MCP794xx::setTime12(int hour, bool _PM, int minute, int second)
{
	// Conform Input values
	if (hour > 12) hour = 12;
	if (hour < 1) hour = 1;
	if (minute > 59) minute = 59;
	if (minute < 0) minute = 0;
	if (second > 59) second = 59;
	if (second < 0) second = 0;

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) | (_PM ? 0x60 : 0x40));	// write the format and
	Wire.endTransmission();								// stop transmitting
}
void MCP794xx::setTime24(int hour, int minute, int second)
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

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) & 0xBF);					// write the format and
	Wire.endTransmission();								// stop transmitting
}
void MCP794xx::setYear(int year)
{
	// Conform input values
	if (year > 99) year = 99;
	if (year < 0) year = 0;

	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(_dateYearReg);							// Point to Register defined by Address
	Wire.write(decToBcd(year));							// Write the year
	Wire.endTransmission();
}
void MCP794xx::setMonth(int month)
{
	// Conform input to valid values.
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;

	byte dateBuffer = readByte(_dateDateReg);				// Read Date Register
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(_dateDateReg);								// Point to Register defined by Address
	Wire.write(dateBuffer);									// Rewrite Date register, Errata Fix.
	Wire.write(decToBcd(month));							// Write the month buffer with lear year bit
	Wire.endTransmission();

}
void MCP794xx::setDate(int date)
{
	// Conform input values, here best we can do it prevent crazy dates from happening.
	if (date > 31) date = 31;
	if (date < 1) date = 1;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(_dateDateReg);								// Point to Register defined by Address
	Wire.write(decToBcd(date));								// Write the date value
	Wire.endTransmission();									// stop transmitting
}
void MCP794xx::setWeekday(int weekday)
{
	byte regBuff;
	// Conform input values
	if (weekday > _SUN) weekday = _SUN;
	if (weekday < _MON) weekday = _MON;
	regBuff = readByte(_dateWeekdayReg) & 0xf8;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(_dateWeekdayReg);							// Point to Register defined by Address
	Wire.write(regBuff | decToBcd(weekday) | 0x08);					// Write the weekday value and enabling batt. backup.
	Wire.endTransmission();									// stop transmitting
}
void MCP794xx::setCalendar(int year, int month, int date)
{
	// Conform input values
	if (year > 99) year = 99;
	if (year < 0) year = 0;
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;
	if (date > 31) date = 31;
	if (date < 1) date = 1;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(_dateDateReg);								// Point to Register defined by Address
	Wire.write(decToBcd(date));
	Wire.write(decToBcd(month));
	Wire.write(decToBcd(year));
	Wire.endTransmission();
}
int MCP794xx::getHours()
{
	byte hourBuff = readByte(_timeHourReg);
	if (hourBuff & 0x40)
	{
		// 12 hour format
		if (hourBuff & 0x20)
		{
			PM = true;
		}
		else {
			PM = false;
		}
		hourBuff &= 0x1F;				// Mask out right bits
	}
	else {
		// 24 hour format
		PM = false;						// Ensure PM is false.
		hourBuff &= 0x3F;				// Mask out the right bits
	}

	return bcdToDec(hourBuff);			// Return the value of the hour
}
int MCP794xx::getMinutes()
{
	return bcdToDec(readByte(_timeMinReg));
}
int MCP794xx::getSeconds()
{
	return bcdToDec(readByte(_timeSecReg) & 0x7F);
}
int MCP794xx::getYear()
{
	byte data[2];
	Wire.beginTransmission(_MCP794xxaddress);	// transmit to RTC
	Wire.write(_dateMonthReg);					// Point to Register defined by Address
	Wire.endTransmission(false);				// Setup for read
	Wire.requestFrom(_MCP794xxaddress, 2);		// Read two bytes
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
int MCP794xx::getMonth()
{
	return bcdToDec(readByte(_dateMonthReg) & 0x1F);
}
int MCP794xx::getDate()
{
	return bcdToDec(readByte(_dateDateReg));
}
int MCP794xx::getWeekday()
{
	return bcdToDec(readByte(_dateWeekdayReg) & 0x07);
}
void MCP794xx::setAlarmHours12(bool alarmSelect, int hours, bool _PM)
{
	// Conform Input values
	if (hours > 12) hours = 12;
	if (hours < 1) hours = 1;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmHourReg);					// Point to Register defined by Address
	Wire.write(decToBcd(hours) | (_PM ? 0x20 : 0x00));		//
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	configBuff |= _alarmMatchHours;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//
	Wire.endTransmission();									// stop transmittin
}
void MCP794xx::setAlarmHours24(bool alarmSelect, int hours)
{

	// Conform Input values
	if (hours == 24 || hours < 0) {
		hours = 0;
	}
	else if (hours > 23) {
		hours = 23;
	}

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmHourReg);					// Point to Register defined by Address
	Wire.write(decToBcd(hours));							//
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	configBuff |= _alarmMatchHours;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//
	Wire.endTransmission();									// stop transmittin


}
void MCP794xx::setAlarmMinutes(bool alarmSelect, int minutes)
{
	// Conform input values
	if (minutes > 59) minutes = 59;
	if (minutes < 0) minutes = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmMinReg);						// Point to Register defined by Address
	Wire.write(decToBcd(minutes));							//
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte((baseReg + _alarmWeekdayReg)) & 0x87;
	configBuff &= 0x80;
	configBuff |= _alarmMatchMinutes;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//
	Wire.endTransmission();									// stop transmittin
}
void MCP794xx::setAlarmSeconds(bool alarmSelect, int seconds)
{
	// Conform input values
	if (seconds > 59) seconds = 59;
	if (seconds < 0) seconds = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmSecReg);						// Point to Register defined by Address
	Wire.write(decToBcd(seconds));							//
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	configBuff |= _alarmMatchSeconds;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//
	Wire.endTransmission();									// stop transmittin
}
void MCP794xx::setAlarmWeekday(bool alarmSelect, int weekday)
{
	// Conform input values
	if (weekday > _SUN) weekday = _SUN;
	if (weekday < _MON) weekday = _MON;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;			// Select the alarm base register
	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	Wire.beginTransmission(_MCP794xxaddress);						// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);							// Point to Register defined by Address
	Wire.write(configBuff | decToBcd(weekday) | _alarmMatchWeekday);		//
	Wire.endTransmission();											// stop transmittin
}
void MCP794xx::setAlarmDate(bool alarmSelect, int date)
{
	// Conform input values, here best we can do it prevent crazy dates from happening.
	if (date > 31) date = 31;
	if (date < 1) date = 1;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmDateReg);					// Point to Register defined by Address
	Wire.write(decToBcd(date));								//
	Wire.endTransmission();									// stop transmittin

	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	configBuff |= _alarmMatchDate;

	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg + _alarmWeekdayReg);					// Point to Register defined by Address
	Wire.write(configBuff);									//
	Wire.endTransmission();									// stop transmittin
}
void MCP794xx::setAlarmAll12(bool alarmSelect, int month, int date, int weekday, int hours, bool _PM, int minutes, int seconds)
{
	// Conform Input values
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;
	if (date > 31) date = 31;
	if (date < 1) date = 1;
	if (weekday < _MON) weekday = _MON;
	if (weekday > _SUN) weekday = _SUN;
	if (hours > 12) hours = 12;
	if (hours < 1) hours = 1;
	if (minutes > 59) minutes = 59;
	if (minutes < 0) minutes = 0;
	if (seconds > 59) seconds = 59;
	if (seconds < 0) seconds = 0;

	byte baseReg = alarmSelect ? _alarm1Reg : _alarm0Reg;	// Select the alarm base register
	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg);									// Point to Register defined by Address
	Wire.write(decToBcd(seconds));
	Wire.write(decToBcd(minutes));
	if (_PM) {
		Wire.write(decToBcd(hours) | 0x20);
	}
	else {
		Wire.write(decToBcd(hours));
	}
	Wire.write(configBuff | _alarmMatchAll | decToBcd(weekday));
	Wire.write(decToBcd(date));
	Wire.write(decToBcd(month));
	Wire.endTransmission();									// stop transmittin
}
void MCP794xx::setAlarmAll24(bool alarmSelect, int month, int date, int weekday, int hours, int minutes, int seconds)
{

	// Conform Input values
	if (month > _DEC) month = _DEC;
	if (month < _JAN) month = _JAN;
	if (date > 31) date = 31;
	if (date < 1) date = 1;
	if (weekday < _MON) weekday = _MON;
	if (weekday > _SUN) weekday = _SUN;
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
	byte configBuff = readByte(baseReg + _alarmWeekdayReg);
	configBuff &= 0x80;
	Wire.beginTransmission(_MCP794xxaddress);				// transmit to RTC
	Wire.write(baseReg);									// Point to Register defined by Address
	Wire.write(decToBcd(seconds));
	Wire.write(decToBcd(minutes));
	Wire.write(decToBcd(hours));
	Wire.write(configBuff | _alarmMatchAll | decToBcd(weekday));
	Wire.write(decToBcd(date));
	Wire.write(decToBcd(month));
	Wire.endTransmission();									// stop transmittin
}
void MCP794xx::enableAlarm(bool alarmSelect)
{
	byte configBuff = readByte(_sysCtrlReg);
	if (alarmSelect)
	{
		configBuff |= 0x20;
	}
	else {
		configBuff |= 0x10;
	}
	//configBuff |= alarmSelect ? 0x20 : 0x10;
	writeByte(_sysCtrlReg, configBuff);
}
void MCP794xx::clearFlag(bool alarmSelect)
{
	byte regBuff;
	if (alarmSelect) {
		regBuff = readByte(_alarm1WeekdayReg);
		writeByte(_alarm1WeekdayReg, (regBuff & 0xf7)); // Clear IF
	}
	else {
		regBuff = readByte(_alarm0WeekdayReg);
		writeByte(_alarm0WeekdayReg, (regBuff & 0xf7)); // Clear IF
	}

}
void MCP794xx::setAlarmPolarity(bool polarity)
{
	byte regBuff = readByte(_alarm0WeekdayReg) & 0x7F; // Clear ALMPOL bit
	if (polarity)
	{
		regBuff &= 0x80;
	}
	writeByte(_alarm0WeekdayReg, regBuff);
}
void MCP794xx::disableAlarm(bool alarmSelect)
{
	byte configBuff = readByte(_sysCtrlReg);
	configBuff &= alarmSelect ? 0xDF : 0xEF;
	writeByte(_sysCtrlReg, configBuff);
}
byte MCP794xx::checkAlarm()
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
				flags |= _0matchSec;						// Set flags

				break;
			case 0x10:
				// Match on minutes

				// Double check for minute match as per Errata.
				minBuff = readByte(_timeMinReg);			// Read Minute Register
				regBuff = readByte(_alarm0MinReg);			// Read Alarm Minute Register

				if (minBuff == regBuff) flags |= _0matchMin;	// Set flags

				break;
			case 0x20:
				// Match on hours
				flags |= _0matchHours;						// Set flags

				break;
			case 0x30:
				// Match on Day of the week
				flags |= _0matchWeekday;					// Set flags

				break;
			case 0x40:
				// Match on the day of the month
				flags |= _0matchDate;						// Set flags

				break;
			case 0x70:
				// Match on Seconds, Minutes, Hours, Day of the Week, Date, and Month.
				flags |= _0matchAll;						// Set flags

			default:
				break;
			}
			regBuff = readByte(_alarm0WeekdayReg);
			writeByte(_alarm0WeekdayReg, (regBuff & 0xf7)); // Clear IF
			disableAlarm(0);
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
				flags |= _1matchSec;						// Set flags

				break;
			case 0x10:
				// Match on minutes

				// Double check for minute match as per Errata.
				minBuff = readByte(_timeMinReg);			// Read Minute Register
				regBuff = readByte(_alarm1MinReg);			// Read Alarm Minute Register

				if (minBuff == regBuff) flags |= _1matchMin;	// Set flags

				break;
			case 0x20:
				// Match on hours
				flags |= _1matchHours;						// Set flags

				break;
			case 0x30:
				// Match on Day of the week
				flags |= _1matchWeekday;					// Set flags

				break;
			case 0x40:
				// Match on the day of the month
				flags |= _1matchDate;						// Set flags

				break;
			case 0x70:
				// Match on Seconds, Minutes, Hours, Day of the Week, Date, and Month.
				flags |= _1matchAll;						// Set flags
			default:
				break;
			}
			regBuff = readByte(_alarm1WeekdayReg);
			writeByte(_alarm1WeekdayReg, (regBuff & 0xf7)); // Clear IF
			disableAlarm(1);
		}
		break;
	default:
		break;
	}
	return flags;
}
int MCP794xx::getPwrDownHours()
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
int MCP794xx::getPwrDownMinutes()
{
	return bcdToDec(readByte(_pwrDnMinReg));
}
int MCP794xx::getPwrDownMonth()
{
	return bcdToDec(readByte(_pwrDnMonthReg) & 0x1F);
}
int MCP794xx::getPwrDownDate()
{
	return bcdToDec(readByte(_pwrDnDateReg));
}
int MCP794xx::getPwrDownWeekday()
{
	return bcdToDec((readByte(_pwrDnMonthReg) & 0xE0) >> 5);
}
int MCP794xx::getPwrUpHours()
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
void MCP794xx::armPwrTimestamp()
{
	byte regBuff = readByte(_dateWeekdayReg) & 0xEF;
	writeByte(_dateWeekdayReg, regBuff);

}
int MCP794xx::getPwrUpMinutes()
{
	return bcdToDec(readByte(_pwrUpMinReg));
}
int MCP794xx::getPwrUpMonth()
{
	return bcdToDec(readByte(_pwrUpMonthReg) & 0x1F);
}
int MCP794xx::getPwrUpDate()
{
	return bcdToDec(readByte(_pwrUpDateReg));
}
int MCP794xx::getPwrUpWeekday()
{
	return bcdToDec((readByte(_pwrUpMonthReg) & 0xE0) >> 5);
}
void MCP794xx::setMFPin(bool value)
{
	byte configBuff = readByte(_sysCtrlReg);		// Read current configuration
	configBuff &= 0x0F;
	writeByte(_sysCtrlReg, configBuff | (value ? 0x80 : 0x00));

}
void MCP794xx::setMFPinSquareWave(int selectOut)
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
void MCP794xx::writeData(byte reg, byte* buffer, int numBytes)
{
	Wire.beginTransmission(_MCP794xxaddress);			// transmit to RTC
	Wire.write(reg);
	for (int i = 0; i < numBytes;i++)
	{
		Wire.write(buffer[i]);
		if ((reg + i) == _dat0x3F) break;				// Prevent an overflow condition
	}
	Wire.endTransmission();								// stop transmitting
}
void MCP794xx::readData(byte reg, byte* buffer, int numBytes)
{
	Wire.beginTransmission(_MCP794xxaddress);	// transmit to RTC
	Wire.write(reg);							// Point to starting Register
	Wire.endTransmission(false);

	if (numBytes > 32)
	{
		// Arduino's I2C buffer is only 32 bytes.
		// to get around this we perform the large read in segments.
		for (int j = 0; j < (numBytes / 32); j++)
		{
			Wire.requestFrom(_MCP794xxaddress, numBytes);
			for (int i = 0; i < numBytes; i++)
			{
				buffer[(j * 32) + i] = Wire.read();
				if ((reg + (j * 32) + i) >= _dat0x3F) break;				// Prevent an overflow condition
			}
		}
	}
	else {
		Wire.requestFrom(_MCP794xxaddress, numBytes);
		for (int i = 0; i < numBytes;i++)
		{
			buffer[i] = Wire.read();
			if ((reg + i) >= _dat0x3F) break;				// Prevent an overflow condition
		}
	}
	Wire.endTransmission();								// stop transmitting
}
void MCP794xx::standbyMode()
{
	// May be left unimplemented
}

/////////////////////
// Private Functions
/////////////////////

void MCP794xx::writeByte(byte address, byte data) {
	Wire.beginTransmission(_MCP794xxaddress);   // transmit to RTC
	Wire.write(address);            // Point to Register defined by Address
	Wire.write(data);               // Write data
	Wire.endTransmission();         // stop transmitting
	return;
}
byte MCP794xx::readByte(byte address) {
	byte data;

	Wire.beginTransmission(_MCP794xxaddress);   // transmit to RTC
	Wire.write(address);            // Point to Register defined by Address
	Wire.endTransmission(false);

	Wire.requestFrom(_MCP794xxaddress, 1);

	data = Wire.read();            // Read the register

	Wire.endTransmission();

	return data;
}
byte MCP794xx::bcdToDec(byte bcd)
{

	return(((bcd >> 4) * 10) + (bcd & 0xF));
}
byte MCP794xx::decToBcd(byte val)
{
	return(((val / 10) << 4) | (val % 10));
}
