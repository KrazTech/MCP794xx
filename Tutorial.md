# Table of Contents
* [Introduction](#introduction)
* [Setting up the library](#librarySetup)
* [Initializing](#initialize)
* [Setting the Time](#timeSet)
    * [Just set the time](#timeJustSet)
        * [24 hour format](#timeJustSet24H)
        * [12 hour format](#timeJustSet12H)
    * [Setting the Hour](#timeJustSet)
        * [24 hour format](#timeSetHour24H)
        * [12 hour format](#timeSetHour12H)
    * [Setting the minutes](#timeSetMinute)
    * [Setting the seconds](#timeSetSecond)
* [Setting the Calendar](#dateSetCalendar)
    * [Just set the Calendar](#dateJustSetCalendar)
    * [Setting the Year](#dateSetYear)
    * [Setting the Month](#dateSetMonth)
    * [Setting the Date](#dateSetDate)
    * [Setting the Weekday](#dateSetWeekday)
* [Reading the Time](#timeRead)
    * [Reading the Hour](#timeReadHour)
        * [24 hour format](#timeReadHour24H)
        * [12 hour format](#timeReadHour12H)
    * [Reading the Minute](#timeReadMinute)
    * [Reading the Second](#timeReadSecond)
* [Reading the Calendar](#dateReadCalendar)
    * [Reading the Year](#dateReadYear)
    * [Reading the Month](#dateReadMonth)
    * [Reading the Date](#dateReadDate)
    * [Reading the Weekday](#dateReadWeekday)
* [Multifunction Pins](#MFP)
    * [Square Wave Output](#MFPSquareWaveOutput)
    * [General Purpose Output](#MFPGeneralPurpose)
* [Alarms]
* [Storing Data]
* [Reading Data]

<a name="introduction"></a>
# Introduction
Thanks for either buying a Kraztech MCP794xx RTC module, or simply to be using my Arduino library written for the Microchip MCP794xx RTC Series. This guide is intended as a tutorial going through the entire Arduino library's use. By the end you should be able to fully
utilize the functionality found on the Kraztech MCP794xx RTC Module.

<a name="librarySetup"></a>
# Setting up the library
Adding the MCP794xx library to your Arduino Environment isn't much of a task. The first step you will want to do is to copy the library files under this repositories `/src` directory. These files should then be places under your Arduino's library folder. Typically this folder is located at: `~/Documents/Arduino/libraries`.
By the end, you should have a folder containing both the `MCP794xx.cpp` and the `MCP794xx.h` files. Personally, I simply cloned the Git Repository and copied that folder into my Arduino library folder under `My Documents`. Here's what my directory ended up looking like:
```
~/Documents/Arduino/libraries/MCP794xx/examples/Examples.ino
~/Documents/Arduino/libraries/MCP794xx/src/MCP794xx.cpp
~/Documents/Arduino/libraries/MCP794xx/src/MCP794xx.h
~/Documents/Arduino/libraries/MCP794xx/.gitignore
~/Documents/Arduino/libraries/MCP794xx/library.properties
~/Documents/Arduino/libraries/MCP794xx/LICENSE
~/Documents/Arduino/libraries/MCP794xx/README.md
~/Documents/Arduino/libraries/MCP794xx/Tutorial.md
```
Now, most of these files aren't required, the ones you really need are the files under the `/src` directory to make the library work.

<a name="initialize"></a>
# Initializing
The MCP794xx library uses a class object to interact with the module via I2C. The very first step in interacting with the RTC is to first instantiate the object. All that means is we want to create an MCP794xx object, by example:
```
MCP794xx RTC; // Creates an RTC object

MCP794xx realTimeClock; // Also creates an RTC Object
```
Once the MCP794xx object has been created, you can now call object methods to interact with the physical MCP794xx module.

<a name="timeSet"></a>
# Setting the Time
Lets get started with the one thing an RTC is really meant to do; time. The MCP794xx can operate in both 12 and 24 hour formats. The time can be set in a single method call, or by setting each component (hour, minute, second) separately.
 <a name="timeJustSet"></a>
#### Just set the time
We'll start off by setting the time in a single method call. I would guess that this would be the primary way most people will set the time. It's simple, straight forward, and gets the job done in a single line. Here are some examples of setting the time in both 24 & 12 hour formats.
<a name="timeJustSet24H"></a>
##### 24 Hour Format
```
/// Using method: void setTime24(int hour, int minute, int second);

RTC.setTime24(5, 23, 10); // Sets the time to 5:23:10 (HH:MM:SS)

RTC.setTime24(19, 31, 45); // Sets the time to 19:31:45 (HH:MM:SS)

```
<a name="timeJustSet12H"></a>
##### 12 Hour Format  
```
/// Using method: void setTime12(int hour, bool _PM, int minute, int second);

RTC.setTime12(4, 0, 23, 54); // Sets the time to 4:23:54 am (HH:MM:SS)

RTC.setTime12(8, false, 42, 11); // Sets the time to 8:42:11 am (HH:MM:SS)

RTC.setTime12(5, 1, 32, 12); // Sets the time to 5:32:12 pm (HH:MM:SS)

RTC.setTime12(10, true, 11, 43); // Sets the time to 10:11:43 pm (HH:MM:SS)
```
<a name="timeSetHour"></a>
## Setting the hour
<a name="timeSetHour24H"></a>
### 24 Hour format
Setting up the Module to operate in 24 hour format is quite easy. The only thing that really determines if the RTC is operating in either 12 or 24 hour format is, well, the hour! Therefore the only difference between the two operations are the methods called to set the hour (at least for the scope of setting time). Here's an example for 24 hour format:
```
/// Using method: void setHours24(int hour);

RTC.setHours24(0); // Sets the hour to 00:mm:ss

RTC.setHours24(14); // Sets the hour to 14:mm:ss
```
<a name="timeSetHour12H"></a>
### 12 Hour format
Setting up the module to operate in 12 hour format is, again, straight forward. Compared to the 24 hours format, we do need to account for if the hour is in the PM or AM. This is communicated by using a boolean parameter in the method call. Here's an example:
```
/// Using method: void setHours12(int hour, bool _PM);

RTC.setHours12(4, 0); // Sets the hour to 4:mm:ss am

RTC.setHours12(11, false); // Sets the hour to 11:mm:ss am

RTC.setHours12(5, 1); // Sets the hour to 5:mm:ss pm

RTC.setHours12(9, true); // Sets the hour to 9:mm:ss pm
```
<a name="timeSetMinute"></a>
## Setting the minutes
Setting the minutes on the RTC module is the same for both hour formats. Here are a couple examples:
```
/// Using method: void setMinutes(int minute);

RTC.setMinutes(32); // Sets the minute to hh:32:ss

RTC.setMinutes(12); // Sets the minute to hh:12:ss
```
<a name="timeSetSecond"></a>
## Setting the seconds
Setting the seconds on the RTC module is the same for both hour formats. Here are a couple examples:
```
/// Using method: void setSeconds(int second);

RTC.setSeconds(42); // Sets seconds to hh:mm:42

RTC.setSeconds(12); // Sets seconds to hh:mm:12
```
<a name="dateSetCalendar"></a>
# Setting the Calendar
Another primary feature of the MCP794xx RTC module is the date. Like most RTCs out there, we can accurately keep track of the date (at least until 2399!) along side time. The Kraztech library can set the date in one go (expect the weekday), or individually by each component (year, month, day).
<a name="dateJustSetCalendar"></a>
#### Just Setting the Calendar
Here we can set the calendar's year, month, and date in a single function call. Note that the enumeration used in the following example is fully described under the *Setting the Month* section. Here are a couple examples:
```
/// Using method: void setCalendar(int year, int month, int date);

RTC.setCalendar(18, 11, 23);  // Sets the date to year xx18, November the 23'rd

RTC.setCalendar(23, _APR, 6); // Sets the date to year xx23, April the 6'th
```
<a name="dateSetYear"></a>
## Setting the Year
Setting the year is done by tracking the last 2 digits of the year. For example, if we want to set the year to **2018**, we would set the year to **18** to indicate the last two digits of that year. We can't keep track of which century or millennia is it; darn. Here are a couple examples showing how we can set the year:
```
/// Using method: void setYear(int year);

RTC.setYear(56);  // Sets the year to xx56

RTC.setYear(13);  // Sets the year to xx13
```
<a name="dateSetMonth"></a>
## Setting the Month
Setting the month is just as simple as everything else. But the exciting thing about setting is the month is that we can use either the numerical value for the month (1-12) or an enumeration to make things a bit more readable. Here is a list of the numerations that we can use to set the date:

|Month|Numerical Equivalent|
|-----|:---:|
|_JAN|1|
|_FEB|2|
|_MAR|3|
|_APR|4|
|_MAY|5|
|_JUN|6|
|_JUL|7|
|_AUG|8|
|_SEPT|9|
|_OCT|10|
|_NOV|11|
|_DEC|12|

Now here are a couple examples of setting the month:
```
/// Using method: void setMonth(int month);

RTC.setMonth(7);    // Sets the month to July

RTC.setMonth(_AUG); // Sets the month to August
```
<a name="dateSetDate"></a>
## Setting the Date
Onwards to setting the date. Here we're talking about the date of the month (1-31). Just like with everything else, it's straight forward. Here are a couple examples:
```
/// Using method:	void setDate(int date);

RTC.setDate(12);  // Sets the date to the 12'th of the month

RTC.setDate(20);  // Sets the date to the 20'th of the month
```
<a name="dateSetWeekday"></a>
## Setting the Weekday
The weekday is an additional calendar component that the RTC can keep track of. The connection between which day of the week it is and the rest of the calendar is a little loose. The day of the week will increment on a day roll over (when the hour goes from 23->0 (24h format) or 11pm -> 12am(12h format)). Allowing us to align the day of the week which ever way we would like. All this means is that November 1'st can be set to Monday, Tuesday, Wednesday, Thursday, Friday,... I think you get the point. While we can set the day of the week via numerical values (1-7), this can quickly become confusing. So the library includes an enumeration to simplify keeping track of the weekday. Here is a list of the enumeration:

|Day of the Week|Numerical Equivalent|
|:-----:|:---:|
|_MON|1|
|_TUE|2|
|_WED|3|
|_THUR|4|
|_FRI|5|
|_SAT|6|
|_SUN|7|

With that out of the way, here are a couple of examples of setting the day of the week:
```
/// Using method: void setWeekday(int weekday);

RTC.setWeekday(1);    // Sets the weekday to 1, with the enum it's Monday

RTC.setWeekday(_FRI); // Sets the weekday to Friday
```
<a name="timeRead"></a>
# Reading the Time
Reading the time off of the MCP794xx RTC Module is done by time component; hour, minute, second. The values are then returned by the function and therefore must be stored in a variable. Let's go through reading each time component off of the RTC.

<a name="timeReadHour"></a>
## Reading the Hour
Reading the hour for both the 24 hour & 12 hour formats are the same. The only difference is in the 12 hour format, the status of the local **PM** class variable is updated with the RTC's **PM** status. Here are some examples showing the behaviour you would expect from the library:
<a name="timeReadHour24H"></a>
#### 24 hour format
```
/// Using method: int getHours();

int hour; // Variable for the hour

RTC.setHours24(3); // Sets the hour to 03:xx

hour = RTC.getHours(); // Returns the value 3 and stores it in hour.
```
<a name="timeReadHour12H"></a>
#### 12 hour format
```
/// Using method: int getHours();

int hour; // Variable for the hour


RTC.setHours12(6, true); // Sets the hour to 6 pm

hour = RTC.getHours(); // Returns the value of 6 and stores it in hour.

// RTC.PM will now be SET or "true" indicating that the hour was in the PM



RTC.setHours12(6, false); // Sets the hour to 6 am

hour = RTC.getHours(); // Returns the value of 6 and stores it in hour.

// RTC.PM will now be UNSET or "false" indicating that the hour was in the AM

```
<a name="timeReadMinute"></a>
## Reading the Minute
Reading the minutes slightly more straight forward than reading the hour because we're not dealing with the PM status bit. Here are a couple examples:
```
/// Using method: int getMinutes();

int minute; // Variable for minute


RTC.setMinutes(34); // Sets the minute to xx:34

minute = RTC.getMinutes(); // Returns 34 and stores it in minute



RTC.setMinutes(11); // Sets the minute to xx:11

minute = RTC.getMinutes(); // Returns 11 and stores it in minute
```
<a name="timeReadSecond"></a>
## Reading the Second
Reading the seconds is as straight forward as reading the minute. Here are a couple examples:
```
/// Using method: int getSeconds();

int second; // Variable for second


RTC.setSeconds(34); // Sets the second to xx:xx:34

second = RTC.getSeconds(); // Returns 34 and stores it in second



RTC.setSeconds(11); // Sets the second to xx:xx:11

second = RTC.getSeconds(); // Returns 11 and stores it in second
```
<a name="dateReadCalendar"></a>
# Reading the Calendar
Reading the calendar is the same as reading time.

<a name="dateReadYear"></a>
## Reading the Year
```
/// Using method: int getYear();

int year; // Variable for year


RTC.setYear(34); // Sets the year to xx34

year = RTC.getYear(); // Returns 34 and stores it in year
```
<a name="dateReadMonth"></a>
## Reading the Month
```
/// Using method: int getMonth();

int month; // Variable for month


RTC.setMonth(4); // Sets the month to April

month = RTC.getMonth(); // Returns 4 and stores it in month


RTC.setMonth(_NOV); // Sets the month to November

month = RTC.getMonth(); // Returns 11 (_NOV) and stores it in month
```
<a name="dateReadDate"></a>
## Reading the Date
```
/// Using method: int getDate();

int date; // Variable for date


RTC.setDate(26); // Sets the 26'th of the month

date = RTC.getDate(); // Returns 26 and stores it in date
```
<a name="dateReadWeekday"></a>
## Reading the Weekday
```
/// Using method: int getWeekday();

int weekday; // Variable for weekday


RTC.setWeekday(2); // Sets the weekday to 2, (_TUE in this library)

weekday = RTC.getWeekday(); // Returns 2 and stores it in weekday


RTC.setWeekday(_FRI); // Sets the weekday to _FRI

weekday = RTC.getWeekday(); // Returns 5 (_FRI) and stores it in weekday
```
<a name="MFP"></a>
# MFP
The Multifunction Pin (MFP) is an output pin which the MCP794xx Module uses as either a General Purpose Output (we set the output value manually), an Alarm Interrupt Output (the pin becomes Asserted when an alarm goes off), or as a Square Wave Clock Output (a small selection of output frequencies). While it can perform all of these tasks, it can be configured for **one** at any time.

When configuring the MFP, you must be mindful that enabling one feature may be, or could be, overridden by another feature. The hierarchy of which feature takes priority is described in the following through table:

|Square Wave Output Enabled|Alarm 0 Enabled|Alarm 1 Enabled|Mode|
|:-:|:-:|:-:|:-:|
|0|0|0|General Purpose Output|
|0|0|1|Alarm Interrupt Output|
|0|1|0|Alarm Interrupt Output|
|0|1|1|Alarm Interrupt Output|
|1|x|x|Sqaure Wave Output|

To put the table into plain english, all it says is:
1. If the Square wave output is enabled, it doesn't matter what else is enabled, the Square Wave output takes precedent on the MFP.
2. If one of the alarms are enabled, the MFP will be attached to their interrupt output signal.
3. If neither the Square Wave Output nor the Alarms are enabled, then the MFP will default to a general purpose output pin which can be set via software.

<a name="MFPSquareWaveOutput"></a>
### Square Wave output
Since the Square Wave output will take precedent over all other functionality, we'll cover that first. The square wave output can be set to 4 different output frequencies which are connected to Enumerations for simplicity.

|Enumeration|Output Frequency|
|:-:|:-:|
|_32kHz|32.768 kHz|
|_8kHz|8.192 kHz|
|_4kHz|4.096 kHz|
|_1Hz|1 Hz|
|_OFF|(Disables SquareWave Output)

With all the ground work laid out, configuring the Square Wave output is shown here:
```
/// Using method: void setMFPinSquareWave(int selectOut);

RTC.setMFPinSquareWave(_32kHz); // Sets the MFP with the square wave output at 32.768 kHz

RTC.setMFPinSquareWave(_8kHz);  // Sets the MFP with the square wave output at 8.192 kHz

RTC.setMFPinSquareWave(_4kHz);  // Sets the MFP with the square wave output at 4.096 kHz

RTC.setMFPinSquareWave(_1Hz);   // Sets the MFP with the square wave output at 1 Hz

RTC.setMFPinSquareWavE(_OFF);   // Disables the Squarewave output
```

Remember that once the Square Wave is activated it will take control of the MFP until disabled. Meaning that it must be disabled before the alarms will assert any interrupt signals on the MFP.

<a name="MFPGeneralPurpose"></a>
### General Purpose output
Before getting into the involved topic of using the Alarms, we'll quickly cover the use of general purpose output. This just lets us to set the MFP to either a logic high (1 or Vcc) or logic low (0 or 0 V). Here's some example code:
```
/// Using method: void setMFPin(bool value);

RTC.setMFPin(0); // Sets the MFP pin to 0 V

RTC.setMFPin(1); // Sets the MFP pin to Vcc
```
<a name="Alarms"></a>
# Alarms
## Alarm Basics
The alarm functionality of the MCP794xx series RTCs can be simple, or a bit more involved. The Kraztech MCP794xx module has two alarms. They can work separately, or in conjunction with each other for some more advanced functionality.
# Storing Data
# Reading Data
