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

#include "MCP7940.h"

////////////////////
// code section
////////////////////

MCP7940::MCP7940() {
	Serial.begin(115200);	// Can delete after debugged.
	Wire.begin();			// Makes sure the I2C module is operating
}
void MCP7940::start() {

	byte config = readByte(0x00) | 0x80;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(config);									// Write data
	Wire.endTransmission();								// stop transmitting

	setBattOn();										// Turn the battery on by default

}
void MCP7940::stop() {

	byte config = readByte(0x00) & 0x7F;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(config);									// Write data
	Wire.endTransmission();								// stop transmitting
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
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeHourReg);							// Point to Register defined by Address
	Wire.write(decToBcd(hour) | (_PM ? 0x60 : 0x40));	// write the format and 
	Wire.endTransmission();								// stop transmitting

}
void MCP7940::setHours24(int hour)
{
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeHourReg);							// Point to Register defined by Address
	Wire.write(decToBcd(hour) & 0xBF);	// write the format and 
	Wire.endTransmission();
}
void MCP7940::setMinutes(int minute)
{
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeMinReg);							// Point to Register defined by Address
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setSeconds(int second)
{
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setTime12(int hour, bool _PM, int minute, int second)
{
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) | (_PM ? 0x60 : 0x40));	// write the format and 
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setTime24(int hour, int minute, int second)
{
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) & 0xBF);					// write the format and 
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::setYear(int year, bool leapYear)
{
	byte monthBuffer = readByte(_dateMonthReg);		// Read the month value to prevent byte corruption
	if (leapYear) {
		monthBuffer |= 0x20;							// Set leap year bit
	}
	else {
		monthBuffer &= 0xDF;							// Unset the leap year bit
	}
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_dateMonthReg);							// Point to Register defined by Address
	Wire.write(monthBuffer);							// Write the month buffer with lear year bit set
	Wire.write(decToBcd(year));							// Write the year
	Wire.endTransmission();
}
void MCP7940::setMonth(int month)
{
	byte monthBuffer = readByte(_dateMonthReg) & 0x20;	// Read the leap year bit to prevent bit corruption
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateMonthReg);								// Point to Register defined by Address
	Wire.write(decToBcd(month) | monthBuffer);				// Write the month buffer with lear year bit
	Wire.endTransmission();
}
void MCP7940::setDate(int day)
{
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateDayReg);								// Point to Register defined by Address
	Wire.write(decToBcd(day));								// Write the day value
	Wire.endTransmission();									// stop transmitting
}
void MCP7940::setWeekday(int weekday)
{
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateWeekdayReg);							// Point to Register defined by Address
	Wire.write(decToBcd(weekday) | 0x08);					// Write the weekday value and enabling batt. backup.
	Wire.endTransmission();									// stop transmitting
}
void MCP7940::setCalendar(int year, bool leapYear, int month, int day)
{
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateDayReg);								// Point to Register defined by Address
	Wire.write(decToBcd(day));
	if (leapYear)
	{
		Wire.write(decToBcd(month) | 0x20);
	}
	else {
		Wire.write(decToBcd(month));
	}
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
	return bcdToDec(readByte(_pwrDnMonthReg) & 0xE0);
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
	return bcdToDec(readByte(_pwrUpMonthReg) & 0xE0);
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
	if (numBytes > 32)
	{
		for (int j = 0; j < (numBytes / 32); j++)
		{
			Wire.requestFrom(_MCP7940address, numBytes);
			for (int i = 0; i < numBytes;i++)
			{
				buffer[(j * 32) + i] = Wire.read();
				if ((reg + (j * 32) + i) >= _dat0x3F) break;				// Prevent an overflow condition 
			}
		}
	}
	Wire.endTransmission();								// stop transmitting
}
void MCP7940::standbyMode()
{

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

