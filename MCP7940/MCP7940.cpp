/*
 Name:		MCP7940.cpp
 Created:	5/2/2018 11:39:58 PM
 Author:	Chris Krasnichuk
*/

#include "MCP7940.h"

////////////////////
// code section
////////////////////

MCP7940::MCP7940() {
	Serial.begin(115200);	// Can delete after debugged.
	Wire.begin();			// Makes sure the I2C module is operating
}

// Starts the clock operation of the MCP7940.
void MCP7940::start() {

	byte config = readDataByte(0x00) | 0x80;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) | config);				// Write data
	Wire.endTransmission();								// stop transmitting

}

// Stops the clock operation of the MCP7940.
void MCP7940::stop() {

	byte config = readDataByte(0x00) & 0x7F;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(config);									// Write data
	Wire.endTransmission();								// stop transmitting
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
	byte monthBuffer = readDataByte(_dateMonthReg);		// Read the month value to prevent byte corruption
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
	byte monthBuffer = readDataByte(_dateMonthReg) & 0x20;	// Read the leap year bit to prevent bit corruption
	Wire.beginTransmission(_MCP7940address);				// transmit to RTC
	Wire.write(_dateMonthReg);								// Point to Register defined by Address
	Wire.write(decToBcd(month) | monthBuffer);				// Write the month buffer with lear year bit
	Wire.endTransmission();
}
void MCP7940::setDay(int day)
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
void MCP7940::setDate(int year, bool leapYear, int month, int day)
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
int MCP7940::getDay()
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

}

void MCP7940::disableAlarm(bool alarmSelect)
{

}

int MCP7940::getPwrDownHours()
{

}
int MCP7940::getPwrDownMinutes()
{

}
int MCP7940::getPwrDownSeconds()
{

}
int MCP7940::getPwrDownMonth()
{

}
int MCP7940::getPwrDownDay()
{

}
int MCP7940::getPwrUpHours()
{

}
int MCP7940::getPwrUpMinutes()
{

}
int MCP7940::getPwrUpSeconds()
{

}
int MCP7940::getPwrUpMonth()
{

}
int MCP7940::getPwrUpDay()
{

}
void MCP7940::setMFPin(bool value)
{

}
void MCP7940::setMFPinSquareWave(byte config)
{

}
void MCP7940::attachInterrupt()
{

}
void MCP7940::detachInterrupt()
{

}
void MCP7940::writeData()
{

}
byte MCP7940::readData()
{

}
void MCP7940::standbyMode()
{

}

void MCP7940::setTime(byte format, int hour, int minute, int second) {
	
	byte timeOn = (readDataByte(0x00) & 0x80) ? 1 : 0;
	
	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_timeSecReg);							// Point to Register defined by Address
	Wire.write(decToBcd(second) & 0x7F);				// Write second register and turn time off
	Wire.write(decToBcd(minute));						// Write the minute register
	Wire.write(decToBcd(hour) | format);				// write the format and 
	Wire.endTransmission();								// stop transmitting

	if (timeOn) { start(); }							// Start the time again if it was on.

}

void MCP7940::setDate(bool leapYear, int year, int month, int day, int weekday)
{
	//byte timeOn = (readDataByte(0x00) & 0x80) ? 1 : 0;

	Wire.beginTransmission(_MCP7940address);			// transmit to RTC
	Wire.write(_dateWeekdayReg);						// Point to Register defined by Address
	Wire.write(decToBcd(weekday));
	Wire.write(decToBcd(day));
	if (leapYear)
	{
		Wire.write(decToBcd(month) | 0x20);
	}
	else {
		Wire.write(decToBcd(month));
	}
	
	Wire.write(decToBcd(year));
	Wire.endTransmission();								// stop transmitting
}

void MCP7940::readTime() {

	byte format = readByte(0x02) & 0x40;		// Read the 12h/24h format bit.
	byte hourReg = readByte(_timeHourReg);	

	if ((hourReg & _timeFormatBit) == _24h)			// Check if 24h or 12h time format.
	{
		hour = bcdToDec(hourReg & 0x3F);			// Trasnfer hour
		PM = NULL;
	}
	else {
		PM = (hourReg & _PM) ? 1 : 0;				// Check if AM or PM
		hour = bcdToDec(hourReg & 0x1F);			// Transfer hour
	}

	minute = bcdToDec(readDataByte(_timeMinReg));			// transfer minute
	second = bcdToDec(readDataByte(_timeSecReg) & 0x7F);	// transfer seconds
	
}

void MCP7940::readDate() {
	
	byte buffer = 0x00;

	Wire.beginTransmission(_MCP7940address);   // transmit to RTC
	Wire.write(_dateWeekdayReg);
	Wire.endTransmission(false);

	Wire.requestFrom(_MCP7940address, 4);

	weekday = bcdToDec(Wire.read() & 0x07);
	date = bcdToDec(Wire.read());
	buffer = Wire.read();
	month = bcdToDec(buffer & 0x1F);

	(buffer & 0x20) ? leapYear = true : leapYear = false;
	
	year = bcdToDec(Wire.read());

	Wire.endTransmission();
}


void MCP7940::setAlarmAll(byte alarm, int month, int day, int weekday, int hour, int minute, int second, byte PM)
{
	byte regBuffer = readDataByte(_sysCtrlReg) & B10001111; // Read Ctrl reg and clear alarm bits.
	byte hourFormat = readDataByte(_timeHourReg) & _timeFormatBit;
	
	Wire.beginTransmission(_MCP7940address);	// Begin transmission
	Wire.write(alarm);							// Point to Register defined by Address
	Wire.write(decToBcd(second));               // Write seconds
	Wire.write(decToBcd(minute));				// Write Minutes

	if (hourFormat == _12h) 
	{
		Wire.write(decToBcd(hour) | PM);
	}
	else {
		Wire.write(decToBcd(hour));					// Write hours
	}	

	Wire.write(decToBcd(weekday) | B01110000);	// Write Weekday, and set trigger for ALL
	Wire.write(decToBcd(day));					// Write date
	Wire.write(decToBcd(month));				// Write Month
	Wire.endTransmission();						// Transmit mesage.

	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer | _alarm1Bit);
	}
	else {
		writeData(_sysCtrlReg, regBuffer | _alarm0Bit);
	}
	
}
void MCP7940::setAlarmWeekday(byte alarm, int weekday)
{
	byte regBuffer = readDataByte(_sysCtrlReg) & B10001111; // Read Ctrl reg and clear alarm bits.

	byte writeBuffer = (decToBcd(weekday) & 0x03) | 0x30;

	writeData(alarm + _alarmWeekdayReg, writeBuffer);  // Load match value and set alarm criteria

	// Enable associated Alarm.
	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer | _alarm1Bit);
	}
	else {
		writeData(_sysCtrlReg, regBuffer | _alarm0Bit);
	}
}
void MCP7940::setAlarmDate(byte alarm, int day)
{
	byte regBuffer = readDataByte(_sysCtrlReg) & B10001111; // Read Ctrl reg and clear alarm bits.

	writeData(alarm + _alarmDateReg, decToBcd(day));		// Load Alarm time

	writeData(alarm + _alarmWeekdayReg, 0x40);				// Set Format

	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer | _alarm1Bit);
	}
	else {
		writeData(_sysCtrlReg, regBuffer | _alarm0Bit);
	}
}
void MCP7940::setAlarmHour(byte alarm, int hour, byte PM)
{
	byte regBuffer = readDataByte(_sysCtrlReg) & B10001111; // Read Ctrl reg and clear alarm bits.
	byte hourFormat = readDataByte(_timeHourReg) & _timeFormatBit;


	if (hourFormat == _12h)
	{
		writeData(alarm + _alarmHourReg, decToBcd(hour) | PM);
	}
	else {
		writeData(alarm + _alarmHourReg, decToBcd(hour));				// Write hours
	}

	writeData(alarm + _alarmWeekdayReg, 0x20);				// Set Format

	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer | _alarm1Bit);
	}
	else {
		writeData(_sysCtrlReg, regBuffer | _alarm0Bit);
	}
}
void MCP7940::setAlarmMinute(byte alarm, int minute)
{
	byte regBuffer = readDataByte(_sysCtrlReg) & B10001111; // Read Ctrl reg and clear alarm bits.

	writeData(alarm + _alarmMinReg, decToBcd(minute));		// Load Alarm time

	writeData(alarm + _alarmWeekdayReg, 0x10);				// Set Format

	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer | _alarm1Bit);
	}
	else {
		writeData(_sysCtrlReg, regBuffer | _alarm0Bit);
	}
}
void MCP7940::setAlarmSecond(byte alarm, int second)
{
	byte regBuffer = readDataByte(_sysCtrlReg) & B10001111; // Read Ctrl reg and clear alarm bits.

	writeData(alarm + _alarmSecReg, decToBcd(second));		// Load Alarm time

	writeData(alarm + _alarmWeekdayReg, 0x00);				// Set Format

	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer | _alarm1Bit);
	}
	else {
		writeData(_sysCtrlReg, regBuffer | _alarm0Bit);
	}
}
void MCP7940::disableAlarm(byte alarm)
{
	byte regBuffer = readDataByte(_sysCtrlReg); // Read Ctrl reg and clear alarm bits.

	if (alarm == _ALARM1) {
		writeData(_sysCtrlReg, regBuffer & (~_alarm1Bit));
	}
	else {
		writeData(_sysCtrlReg, regBuffer & (~_alarm0Bit));
	}
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

