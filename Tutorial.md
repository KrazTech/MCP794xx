# Introduction
Thanks for either buying a Kraztech MCP794xx RTC module, or simply to be using my Arduino library written for the Microchip MCP794xx RTC Series. This guide is intended as a tutorial going through the entire Arduino library's use. By the end you should be able to fully utilize the functionality found on the Kraztech MCP794xx RTC Module.
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

# Initializing
The MCP794xx library uses a class object to interact with the module via I2C. The very first step in interacting with the RTC is to first instantiate the object. All that means is we want to create an MCP794xx object, by example:
```
MCP794xx RTC; // Creates an RTC object

MCP794xx realTimeClock; // Also creates an RTC Object
```
Once the MCP794xx object has been created, you can now call object methods to interact with the physical MCP794xx module.
# Setting the Time
Lets get started with the one thing an RTC is really meant to do; time. The MCP794xx can operate in both 12 and 24 hour formats. The time can be set in a single method call, or by setting each component (hour, minute, second) separately.
#### Just set the time
We'll start off by setting the time in a single method call. I would guess that this would be the primary way most people will set the time. It's simple, straight forward, and gets the job done in a single line. Here are some examples of setting the time in both 24 & 12 hour formats.
##### 24 Hour Format
```
/// Using method: void setTime24(int hour, int minute, int second);

RTC.setTime24(5, 23, 10); // Sets the time to 5:23:10 (HH:MM:SS)

RTC.setTime24(19, 31, 45); // Sets the time to 19:31:45 (HH:MM:SS)

```
##### 12 Hour Format
```
/// Using method: void setTime12(int hour, bool _PM, int minute, int second);

RTC.setTime12(4, 0, 23, 54); // Sets the time to 4:23:54 am (HH:MM:SS)

RTC.setTime12(8, false, 42, 11); // Sets the time to 8:42:11 am (HH:MM:SS)

RTC.setTime12(5, 1, 32, 12); // Sets the time to 5:32:12 pm (HH:MM:SS)

RTC.setTime12(10, true, 11, 43); // Sets the time to 10:11:43 pm (HH:MM:SS)
```
## Setting the hour
### 24 Hour format
Setting up the Module to operate in 24 hour format is quite easy. The only thing that really determines if the RTC is operating in either 12 or 24 hour format is, well, the hour! Therefore the only difference between the two operations are the methods called to set the hour (at least for the scope of setting time). Here's an example for 24 hour format:
```
/// Using method: void setHours24(int hour);

RTC.setHours24(0); // Sets the hour to 00:mm:ss

RTC.setHours24(14); // Sets the hour to 14:mm:ss
```
### 12 Hour format
Setting up the module to operate in 12 hour format is, again, straight forward. Compared to the 24 hours format, we do need to account for if the hour is in the PM or AM. This is communicated by using a boolean parameter in the method call. Here's an example:
```
/// Using method: void setHours12(int hour, bool _PM);

RTC.setHours12(4, 0); // Sets the hour to 4:mm:ss am

RTC.setHours12(11, false); // Sets the hour to 11:mm:ss am

RTC.setHours12(5, 1); // Sets the hour to 5:mm:ss pm

RTC.setHours12(9, true); // Sets the hour to 9:mm:ss pm
```
## Setting the minutes
Setting the minutes on the RTC module is the same for both hour formats. Here are a couple examples:
```
/// Using method: void setMinutes(int minute);

RTC.setMinutes(32); // Sets the minute to hh:32:ss

RTC.setMinutes(12); // Sets the minute to hh:12:ss
```
## Setting the seconds
Setting the seconds on the RTC module is the same for both hour formats. Here are a couple examples:
```
/// Using method: void setSeconds(int second);

RTC.setSeconds(42); // Sets seconds to hh:mm:42

RTC.setSeconds(12); // Sets seconds to hh:mm:12
```
# Setting the Calendar
Another primary feature of the MCP794xx RTC module is the date. Like most RTCs out there, we can accurately keep track of the date (at least until 2399!) along side time. The Kraztech library can set the date in one go (expect the weekday), or individually by each component (year, month, day).
#### Just Setting the Calendar
Here we can set the calendar's year, month, and date in a single function call. Note that the enumeration used in the following example is fully described under the *Setting the Month* section. Here are a couple examples:
```
/// Using method: void setCalendar(int year, int month, int date);

RTC.setCalendar(18, 11, 23);  // Sets the date to year xx18, November the 23'rd

RTC.setCalendar(23, _APR, 6); // Sets the date to year xx23, April the 6'th
```
## Setting the Year
Setting the year is done by tracking the last 2 digits of the year. For example, if we want to set the year to **2018**, we would set the year to **18** to indicate the last two digits of that year. We can't keep track of which century or millennia is it; darn. Here are a couple examples showing how we can set the year:
```
/// Using method: void setYear(int year);

RTC.setYear(56);  // Sets the year to xx56

RTC.setYear(13);  // Sets the year to xx13
```
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
## Setting the Date
Onwards to setting the date. Here we're talking about the date of the month (1-31). Just like with everything else, it's straight forward. Here are a couple examples:
```
/// Using method:	void setDate(int date);

RTC.setDate(12);  // Sets the date to the 12'th of the month

RTC.setDate(20);  // Sets the date to the 20'th of the month
```
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
# Reading the Time
# Reading the Calendar
# Alarms
## Alarm Basics
The alarm functionality of the MCP794xx series RTCs can be simple, or a bit more involved. The Kraztech MCP794xx module has two alarms. They can work separately, or in conjunction with each other for some more advanced functionality.
# MFP
# Storing Data
# Reading Data
