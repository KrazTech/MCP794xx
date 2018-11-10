# Introduction
Thanks for either buying a Kraztech MCP794xx RTC module, or simply to be using my Arduino library written for the Microchip MCP794xx RTC Series. This guide is intended as a tutorial going through the entire Arduino library's use. By the end you should be able to fully utilize the functionality found on the Kraztech MCP794xx RTC Module.
# Setting up the library
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
##### 24 Hour Format
```
/// Using method: void setTime24(int hour, int minute, int second);

RTC.setTime24(5, 23, 10); // Sets the time to 5:23:10 (HH:MM:SS)

RTC.setTime24(19, 31, 45); // Sets the time to 19:31:45 (HH:MM:SS)

```
##### 12 Hour Format
```
/// Using method: void setTime12(int hour, bool _PM, int minute, int second);

RTC.setTime12()
```
## Setting the hour
### 24 Hour format
Setting up the Module to operate in 24 hour format is quite easy. The only thing that really determines if the RTC is operating in either 12 or 24 hour format is, well, the hour! Therefore the only difference between the two operations are the methods called to set the hour (at least for the scope of setting time). Here's an example for 24 hour format:
```
/// Using method: void setHours24(int hour);

RTC.setHours24(0); // Sets the hour to 00:xx

RTC.setHours24(14); // Sets the hour to 14:xx
```
### 12 Hour format
Setting up the module to operate in 12 hour format is, again, straight forward. Compared to the 24 hours format, we do need to account for if the hour is in the PM or AM. This is communicated by using a boolean parameter in the method call. Here's an example:
```
/// Using method: void setHours12(int hour, bool _PM);

RTC.setHours12(4, 0); // Sets the hour to 4:xx am

RTC.setHours12(11, false); // Sets the hour to 11:xx am

RTC.setHours12(5, 1); // Sets the hour to 5:xx pm

RTC.setHours12(9, true); // Sets the hour to 9:xx pm
```
## Setting the minutes
Setting the minutes on the RTC module is the same for both hour formats. Here are a couple examples:
```
```
## Setting the seconds
Setting the seconds on the RTC module is the same for both hour formats. Here are a couple examples:
```
```
# Setting the Date
## Setting the year
# Reading the Time
# Alarms
# MFP
# Storing Data
# Reading Data
