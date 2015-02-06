 # Arduino_Clock 

 This code will allow you to power an arduino atmega 328p based mcu. The clock uses three four character seven segment displays connected via I2C protocol via an Adafruit Led backpack product id's(878, 880, 879). The clock stores and reads time via i2c using a ds1307 RTC which is connected via I2C as well. The clock uses the ADC analogue pins to read temperature from a tmp36 sensor. Additionally a Honeywell hih4001 relative humidity sensor is also included in the project. The code will automatically use the temperature to generate a temperature corrected RH reading. 

 The code includes 21 ringtones which are stored in RTTL (Ring Tone Transfer Language), the code to decode the ringtones was written by Brett Hagman. I have modified the code to store the ringtones in progmem thus saving memory. The code also includes an alarm, with an on off switch that is coded to a single throw double pole switch. This switch is a master arm for the alarm.

 The clock has 3 buttons and 3 display modes and 5 set modes. The buttons are brightness, 12/24hour toggle, and display mode. The brightness has 4 levels of brightness.  The display modes are:

 	1. Calendar mode - This mode shows the time, month , date, and year.
 	2. Environment mode - This mode shows the time, temperature and humidity
 	3. Alarm mode - This mode shows the time, month and day, and the alarm set time.

 The clock also has 5 set modes. Note that during the set mode the three buttons have different functions. You enter the set mode by holding down the mode button for 3 seconds. You know you are in set mode because the decimals underneath each digit of a 4 character display will flash on and off two times a second. As in display mode pressing the mode button will change the set mode. Holding the set mode button down for 3 additional seconds will exit set mode. The set modes are as follows:

 1. Set the hours and minutes - In this mode the brightness button becomes the minute adjust press once or hold for faster setting. The 24 hour time select button is used to set the hours.

 2. Month/Day set - In this mode the brightness and the 24/hour mode buttons are pressed to set the day and month respectively. Again holding down the button will scroll through the values at a faster rate.

 3. Year set mode - In this mode the brightness increments the years by 1 and the 24/hour mode button increments the year by 10. Again  holding either of the buttons down will increase the scroll rate.

 4. Alarm set mode - In this mode the alarm can be set. Note the alarm is always displayed in 24/hour time mode to prevent confusion. Again as with time the brightness button will set the minutes and the 24/hour mode button will set the hours.

 5. Alarm ring tone  - This mode will allow you to choose from the 21 ringtones that are available to you and it will play  the first 5 seconds of each ringtone as a preview.
