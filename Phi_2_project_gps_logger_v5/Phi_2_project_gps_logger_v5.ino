/*
Phi-2 shield for Arduino
Program title: interactive GPS logger version 5 
This software displays information from an EM 406A GPS sensor on Phi-1 and Phi-2 shields' LCD screen. It also logs the GPS information to the onboard EEPROM of the shield. 
----------------------------------------
Instructions:
0. Please download phi_interfaces library (http://liudr.wordpress.com/phi_interfaces/) and unzip the files and example subfolders in arduino\sketchbooks\libraries\phi_interfaces\
1. Please download phi_prompt library (http://liudr.wordpress.com/phi_prompt/) and unzip the files and example subfolders in arduino\sketchbooks\libraries\phi_prompt\
2. Please download TinyGPS libraries by Mikal Hart (http://arduiniana.org)
3. Please set the size of you display with "#define lcd_rows" and "#define lcd_columns" so the program will automatically adjust to your display size.

Programmed by Dr. John Liu
Revision: 2/12/2012
Free software for educational and personal uses.
No warrantee!
Commercial use without authorization is prohibited.
Find details of the Phi-1 shield, Phi-2 shield, phhi_interfaces and phi_prompt or contact Dr. Liu at
http://liudr.wordpress.com/phi-1-shield/
http://liudr.wordpress.com/phi-2-shield/
http://liudr.wordpress.com/phi_interfaces/
http://liudr.wordpress.com/phi_prompt/
All rights reserved.
----------------------------------------
List of functions:
* Menu gives you several choices:
* Send to PC: sends recorded GPS information to PC via the USB connection.
Two modes are available:
Verbose mode generates information as shown above.
Non-verbose mode sends only the column labels followed by tab-separated data, ideal for spreasheet programs to import. You may copy and paste.
* Erase EEPROM: erases the EEPROM
* Record: records GPS information, lattitude, longitude, altitude, date, time, speed to EEPROM
* Display: displays the GPS coordinates without recording
* Parameters: allows the user to adjust parameters such as period between two consecutive recordings,, PC data format, to start recording at which data entry and end at which entry.
* Up and down cycle through the menu options.
* Left, right, B act as confirm or enter.
* A escapes and only will preserve the latest value.

Sample GPS data:
Lat/Long(10^-5 deg): 4554724, -9413277
Date(ddmmyy): 281210 Time(hhmmsscc): 134000
Alt(cm): 31750 Speed(mph): 1.81

ToDo:
* Escape while recording/displaying is not programmed into this test code. You're welcome to do so as you desire.
----------------------------------------
Thanks:

RTC code created by matt.joyce@gmail.com

EEPROM code Author: hkhijhe

TinyGPS library by Mikal Hart (http://arduiniana.org)

*/

#include <LiquidCrystal.h>
#include <Wire.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <TinyGPS.h>
#include <phi_interfaces.h>
#include <phi_prompt.h>
#include <SoftwareSerial.h>

//Phi-2 shield LCD pin setting
#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 2
#define LCD_D7 3

//Phi-2 shield buttons and channel pin assignments
#define total_buttons 6
#define buzzer 16
#define btn_u 5
#define btn_d 10
#define btn_l 11
#define btn_r 4
#define btn_b 14
#define btn_a 15

#define lcd_rows 4
#define lcd_columns 20

//#define setRTC
#define EEPROM_size 32768UL // This is the maximal bytes on your eeprom, depending on the chip you use. I'm using 24LC256, with 256KiloBits so 32KiloBytes=32768 bytes.

#define GPS_Tx 12
#define GPS_Rx 13

// Messages used in program.
PROGMEM prog_char msg_00[]="GPS LOGGER V5\nDeveloped by:\nDr.Liu 2/12/12\nhttp://liudr.wordpress.com";

//Programming custom characters. The custom characters will be lost when power is cut.
//In the character definition, you have to use B100000 instead of B0 since you will need to do strcpy_P, which terminates at 0 or B0.
PROGMEM prog_char gps_ch0[]={B110,B100,B100,B110,B100000,B1010,B10101,B10101,0};// cm
PROGMEM prog_char gps_ch1[]={B1110,B11011,B1010,B11011,B1010,B11011,B1010,B1110,0};//Inverted 1
PROGMEM prog_char gps_ch2[]={B10001,B1110,B11110,B11101,B11011,B10111,B100000,B11111,0};//Inverted 2
PROGMEM prog_char gps_ch3[]={B100000,B11101,B11011,B11101,B11110,B1110,B10001,B11111,0};//Inverted 3
PROGMEM prog_char gps_ch4[]={B11101,B11001,B10101,B1101,B100000,B11101,B11101,B11111,0};//Inverted 4
PROGMEM prog_char gps_ch5[]={B10,B100,B1000,B10011,B100,B10,B1,B110,0};// /s
PROGMEM prog_char gps_ch6[]={B100000,B100000,B100,B1110,B11111,B100,B100,B100000,0};//Up arrow
PROGMEM prog_char gps_ch7[]={B100000,B100000,B100,B100,B11111,B1110,B100,B100000,0};//Down arrow
PROGMEM const char *gps_ch_item[] = {gps_ch0, gps_ch1, gps_ch2, gps_ch3, gps_ch4, gps_ch5, gps_ch6, gps_ch1};

boolean recording=false; // This indicates whether the program is recording the GPS info. If it is recording, a symbol "a chip" appears on the bottom right corner of the LCD.
boolean verbose=true; // This indicates the output to PC format. Verbose is for a human to read with "Lat/long:" and such. While not in verbose mode, it transfers faster and is more compatible with a spreadsheet program.
unsigned long period=5; // This is the period in seconds between two consecutive GPS recordings.
unsigned long pointer=0; // This pointer points to the next empty EEPROM byte for storing or next unread EEPROM byte for reading.
unsigned long lower_limit=0; // This is the lower limit to the pointer value. Save mode startssaving from this EEPROM address. If you use less than 4MBit EEPROM, there is no problem. If not, the limits can go beyond the limit of the signed integer taken by the input function.
unsigned long upper_limit=EEPROM_size; // This is the upper limit to the pointer value. Once the pointer is equal to this value, save mode will stop saving and quits to menu.

// The following lines instantiates a button group to control 6 buttons
byte pins[]={btn_u,btn_d,btn_l,btn_r,btn_b,btn_a}; // The digital pins connected to the 6 buttons.
char mapping[]={'U','D','L','R','B','A'}; // This is a list of names for each button.
phi_button_groups my_btns(mapping, pins, total_buttons);

// This serial keypad is for debugging.
phi_serial_keypads debug_keypad(&Serial,115200); 

// The following sets up function keys for phi_prompt library
char up_keys[]={"U"}; ///< All keys that act as the up key are listed here.
char down_keys[]={"D"}; ///< All keys that act as the down key are listed here.
char left_keys[]={"L"}; ///< All keys that act as the left key are listed here.
char right_keys[]={"R"}; ///< All keys that act as the right key are listed here.
char enter_keys[]={"B"}; ///< All keys that act as the enter key are listed here.
char escape_keys[]={"A"}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.

// The following adds all available keypads as inputs for phi_prompt library
multiple_button_input * keypads[]={&my_btns,&debug_keypad,0};

// The following sets up the gps, softserial, and LCD objects
TinyGPS gps;
SoftwareSerial nss(GPS_Tx, GPS_Rx); // Second software version, //NewSoftSerial nss(15, 14); // First software version
LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

void setup()
{
  byte ch_buffer[10]; // This buffer is required for custom characters on the LCD.
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
  Wire.begin(); // initialize wire

// Set the two channels to output to drive the buzzer and LED.
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);

  Serial.begin(115200);
  nss.begin(4800);


//Programming custom characters. The custom characters will be lost when power is cut.
  for (int i=7;i<8;i++)
  {
    strcpy_P((char*)ch_buffer,(char*)pgm_read_word(&(gps_ch_item[i])));
    lcd.createChar(7, ch_buffer);
  }

  show_credit();
}

void loop()
{
  top_menu();
}

void show_credit()
{
  // Display credits
  phi_prompt_struct myLongMsg;

  lcd.clear();
  lcd.noBlink();
  myLongMsg.ptr.msg_P=msg_00; // Assign the address of the text string to the pointer.
  myLongMsg.low.i=0; // Default text starting position. 0 is highly recommended.
  myLongMsg.high.i=strlen_P(msg_00); // Position of the last character in the text string, which is size of the string - 1.
  myLongMsg.step.c_arr[0]=lcd_rows; // rows to auto fit entire screen
  myLongMsg.step.c_arr[1]=lcd_columns; // one col list
  myLongMsg.col=0; // Display the text area starting at column 0
  myLongMsg.row=0; // Display the text area starting at row 0
  myLongMsg.option=0; // Option 0, display classic message, option 1, display message with scroll bar on right.

  text_area_P(&myLongMsg);
}

void gpsdump(TinyGPS &gps);
bool feedgps();
void printFloat(double f, int digits = 2);
