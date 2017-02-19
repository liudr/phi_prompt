/*
.______    __    __   __        .______   .______        ______   .___  ___. .______   .___________.
|   _  \  |  |  |  | |  |       |   _  \  |   _  \      /  __  \  |   \/   | |   _  \  |           |
|  |_)  | |  |__|  | |  |       |  |_)  | |  |_)  |    |  |  |  | |  \  /  | |  |_)  | `---|  |----`
|   ___/  |   __   | |  |       |   ___/  |      /     |  |  |  | |  |\/|  | |   ___/      |  |     
|  |      |  |  |  | |  |       |  |      |  |\  \----.|  `--'  | |  |  |  | |  |          |  |     
| _|      |__|  |__| |__|  _____| _|      | _| `._____| \______/  |__|  |__| | _|          |__|     

 _______  .______                 __       __   __    __  
|       \ |   _  \               |  |     |  | |  |  |  | 
|  .--.  ||  |_)  |              |  |     |  | |  |  |  | 
|  |  |  ||      /               |  |     |  | |  |  |  | 
|  '--'  ||  |\  \----.    __    |  `----.|  | |  `--'  | 
|_______/ | _| `._____|   (__)   |_______||__|  \______/  

http://liudr.wordpress.com
*/

//Phi-2 shield buttons
#define btn_u 5
#define btn_d 10
#define btn_l 11
#define btn_r 4
#define btn_b 14
#define btn_a 15

//Phi-2 shield LCD pin setting
#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 2
#define LCD_D7 3

// I tested my code on 20X4 display and 16X2 display. If you have other sizes, you're more than welcome to test this library and get back to me.
#define lcd_rows 4
#define lcd_columns 20

#include <LiquidCrystal.h>
#include <WProgram.h>
#include <phi_buttons.h>
#include <phi_prompt.h>

// Test for sample list
PROGMEM prog_char pasta_00[]="Spaghetti";
PROGMEM prog_char pasta_01[]="Rotelle";
PROGMEM prog_char pasta_02[]="Rotini";
PROGMEM prog_char pasta_03[]="Fettuccine";
PROGMEM prog_char pasta_04[]="Lasagne";
PROGMEM prog_char pasta_05[]="Penne";
PROGMEM prog_char pasta_06[]="Rigatoni";
PROGMEM const char *pasta_items[]= {pasta_00,pasta_01,pasta_02,pasta_03,pasta_04,pasta_05,pasta_06};

PROGMEM prog_char db_00[]="Airlock failure";
PROGMEM prog_char db_01[]="Brain parasite";
PROGMEM prog_char db_02[]="Sonic diarrhea";
PROGMEM prog_char db_03[]="Vacuum hypothermia";
PROGMEM prog_char db_04[]="Alien fastfood";
PROGMEM prog_char db_05[]="Space travel jetleg";
PROGMEM prog_char db_06[]="Dark matter exposure";
PROGMEM const char *db_items[]= {db_00,db_01,db_02,db_03,db_04,db_05,db_06};

PROGMEM prog_char long_msg_p[]="Phi_prompt is the very first Text-based User interface developed for Arduino with a character LCD and a set of 6 keys. It offers arduino project developers the freedom to focus on functions they intend to develop and leave all interactions to phi_prompt. With the library, you will be able to show lists on the LCD. The user can browse the list and decide which item to choose. There are many features associated with lists since it is also used to generate multiple-level menus. You can set up menus of infinite levels. You can also display text areas like on a web page with scroll bars to the right as an option. The user will be able to read long messages by scrolling the message up and down. You can also ask the user to enter information such as integers, floating point numbers, strings, password, etc. all on the key pad. All you have to do as a developer is to call these functions and branch your program depending on their return values.";

phi_buttons btn_1(btn_u, LOW);
phi_buttons btn_2(btn_d, LOW);
phi_buttons btn_3(btn_l, LOW);
phi_buttons btn_4(btn_r, LOW);
phi_buttons btn_5(btn_b, LOW);
phi_buttons btn_6(btn_a, LOW);

LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object
phi_buttons *btns[]={&btn_1,&btn_2,&btn_3,&btn_4,&btn_5,&btn_6}; // Actual buttons definitions are in the defs.h

void setup()
{
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,btns,lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object and the phi_buttons objecst. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
}

void loop()
{
  text_area_demo();

// yes_no dialog example
  if(yn_dialog("Ready for distruction?"))
  {
    lcd.clear();
    lcd.print("Alright!");
  }
  else
  {
    lcd.clear();
    lcd.print("You coward!");
  }
  wait_on_escape(2000);

  MXN_list_demo();

  float_demo();

  ok_dialog("Phi_prompt demo is complete.");
  
  horizontal_scroll_demo();
  
}

void text_area_demo()
{
// Display long message scrolling vertically example
  static byte cols=19, rows=3, opt=1, x=0, y=1;
  phi_prompt_struct myLongMsg;

  int user_input; // This is the storage for the integer
  phi_prompt_struct myIntegerInput;

  user_input=cols;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=6; // Lower limit of the number is 0
  myIntegerInput.high.i=20; // Upper limit of the number is 20
  myIntegerInput.step.i=1; // Step size. User choice increments 2 every time you push up and decrements 2 when you press down.
  myIntegerInput.col=7; // Display number at column 7
  myIntegerInput.row=1; // Display number at row 1
  myIntegerInput.width=2; // The number occupies 2 characters space.
  myIntegerInput.option=0; // Option 0, space pad right, option 1, zero pad left, option 2, space pad left.
  lcd.clear();
  lcd.print("Columns:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  cols=user_input;
  
  user_input=rows;
  myIntegerInput.low.i=1; // Lower limit of the number is 0
  myIntegerInput.high.i=4; // Upper limit of the number is 20
  lcd.clear();
  lcd.print("Rows:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  rows=user_input;
  
  user_input=x;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=0; // Lower limit of the number is 0
  myIntegerInput.high.i=19; // Upper limit of the number is 20
  myIntegerInput.width=2; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("x:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  x=user_input;
  
  user_input=y;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.high.i=3; // Upper limit of the number is 20
  myIntegerInput.width=1; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("y:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  y=user_input;
  
  user_input=opt;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=0; // Lower limit of the number is 0
  myIntegerInput.high.i=4; // Upper limit of the number is 20
  myIntegerInput.width=1; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("Display option:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  opt=user_input;

  lcd.clear();
  center_text("Introduction"); // Prompt user for input. Use center_text to display center-aligned.
  myLongMsg.ptr.msg=long_msg_p; // Assign the address of the text string to the pointer.
  myLongMsg.low.i=0; // Default text starting position. 0 is highly recommended.
  myLongMsg.high.i=strlen_P(long_msg_p); // Position of the last character in the text string, which is size of the string - 1.
  myLongMsg.step.c_arr[0]=rows; // row
  myLongMsg.step.c_arr[1]=cols; // column
  myLongMsg.col=x; // Display the text area starting at column 0
  myLongMsg.row=y; // Display the text area starting at row 0
  myLongMsg.option=1; // Option 0, display classic message, option 1, display message with scroll bar on right.

  text_area_P(&myLongMsg);
}

void MXN_list_demo()
{
// Display list
  static byte cols=1, rows=3, len=18, opt=109, x=0, y=1, x2=15, y2=0;
  int user_input; // This is the storage for the integer
  phi_prompt_struct myIntegerInput;

  user_input=cols;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=1; // Lower limit of the number is 0
  myIntegerInput.high.i=4; // Upper limit of the number is 20
  myIntegerInput.step.i=1; // Step size. User choice increments 2 every time you push up and decrements 2 when you press down.
  myIntegerInput.col=7; // Display number at column 7
  myIntegerInput.row=1; // Display number at row 1
  myIntegerInput.width=1; // The number occupies 2 characters space.
  myIntegerInput.option=0; // Option 0, space pad right, option 1, zero pad left, option 2, space pad left.
  lcd.clear();
  lcd.print("Columns:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  cols=user_input;
  
  user_input=rows;
  lcd.clear();
  lcd.print("Rows:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  rows=user_input;
  
  user_input=x;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=0; // Lower limit of the number is 0
  myIntegerInput.high.i=19; // Upper limit of the number is 20
  myIntegerInput.width=2; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("x:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  x=user_input;
  
  user_input=y;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.high.i=3; // Upper limit of the number is 20
  myIntegerInput.width=1; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("y:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  y=user_input;
  
  user_input=x2;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=0; // Lower limit of the number is 0
  myIntegerInput.high.i=19; // Upper limit of the number is 20
  myIntegerInput.width=2; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("x2:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  x2=user_input;
  
  user_input=y2;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.high.i=3; // Upper limit of the number is 20
  myIntegerInput.width=1; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("y2:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  y2=user_input;
  
  user_input=len;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=4; // Lower limit of the number is 0
  myIntegerInput.high.i=20; // Upper limit of the number is 20
  myIntegerInput.width=2; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("Column width:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  len=user_input;
  
  user_input=opt;
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=0; // Lower limit of the number is 0
  myIntegerInput.high.i=127; // Upper limit of the number is 20
  myIntegerInput.width=3; // The number occupies 2 characters space.
  lcd.clear();
  lcd.print("Display option:"); // Prompt user for input
  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  opt=user_input;
  
  int user_choice=0; // This will store the item the user chooses. Item number starts from zero.
  phi_prompt_struct myListInput; // This is the structure you will use to invoke library function select_list.

  myListInput.ptr.list=(char**)&db_items; // Assign the address of the list array to the pointer.
  myListInput.low.i=0; // Default item highlighted on the list is #0, the first item on the list.
  myListInput.high.i=6; // Last item on the list is size of the list - 1.
  myListInput.step.c_arr[0]=rows; // 
  myListInput.step.c_arr[1]=cols; // 
  myListInput.step.c_arr[2]=y2; // 
  myListInput.step.c_arr[3]=x2; // 
  myListInput.width=len; // Length in characters of the longest list item, for screen update purpose.
  myListInput.col=x; // Display the list at column 0
  myListInput.row=y; // Display the list at row 1
  myListInput.option=opt; // Option 3 is an indexed list for clarity. Option 0, display classic list, option 1, display MXN list, option 2, display list with index, option 3, display list with index2.
  lcd.clear(); // Clear the lcd.
  lcd.print("Death by?"); // prompt the user.

  if (select_list(&myListInput)!=-1) // select_list stores user choice in myListInput.low.
  {  
    user_choice=myListInput.low.i; //If the user didn't press escape (return -1) then update the user choice with the value in myListInput.low.

    lcd.noBlink();
    lcd.clear();
    lcd.print("Your choice was:"); // Display the user choice one more time
    myListInput.col=2; // Display the list at column 2
    myListInput.row=1; // Display the list at row 1
    myListInput.step.c_arr[0]=1; // row 1
    myListInput.step.c_arr[1]=1; // column 1
    myListInput.width=14; // Length in characters of the longest list item, for screen update purpose.
    myListInput.option=8; //
    while(!wait_on_escape(250)&&render_list(&myListInput)); // Recall the message stored in PROGMEM and render in on LCD. Auto scroll if needed.
  }
  else 
  {
    lcd.clear();
    lcd.print("You pressed ESC");
  }
  wait_on_escape(2000);
}
  
void float_demo()
{
  char list_buffer[24];

// Input floating number example
  phi_prompt_struct myFloatInput;
  float myFloatNumber=123.45;
  myFloatInput.ptr.f_buffer=&myFloatNumber; // Assign the float number buffer address
  myFloatInput.low.f=10.0; // Lowest valid input
  myFloatInput.high.f=1500.52; // Highest valid input
  myFloatInput.step.c_arr[0]=2; // 2 digits after decimal
  myFloatInput.step.c_arr[1]=4; // 4 digits before decimal

//  myFloatInput.step.i=402; //  and . Note, negative numbers use one digit for negative sign so only 3 digits for numbers if you want to input negative.
  myFloatInput.col=2; // Display input panel at column 2
  myFloatInput.row=1; // Display input panel at row 1
  myFloatInput.option=2; // 0: only positive numbers allowed, 1: only negative numbers allowed, 2: both positive and negative numbers are allowed.

  lcd.clear(); // Clear the lcd
  lcd.print("Input float:"); // Prompt user for input

  if (input_float(&myFloatInput)!=-1) // input_panel stores user choice in file_name.
  {  
    lcd.clear();
    lcd.print("Your input was:"); // Display the user choice one more time
    lcd.setCursor(0,1); // Display the list at column 2 row 1.
    sprintf(list_buffer,"%d.%02d",int(myFloatNumber),int((myFloatNumber-int(myFloatNumber))*100*(myFloatNumber>0?1:-1)+0.5));
    lcd.print(list_buffer);
    Serial.println(myFloatNumber);
  }
  else 
  {
    lcd.clear();
    lcd.print("You pressed ESC");
  }
  wait_on_escape(2000);
}

void horizontal_scroll_demo()
{
// Double horizontal scroll example

  char thankyou[]="Thank you for using phi_prompt!";
  char buffer[15];
  lcd.clear();   
  for (byte i=0;i<47;i++)
  {
    scroll_text(thankyou,buffer,14,i-14);
    lcd.setCursor(1,1);
    lcd.print(buffer);
    scroll_text(thankyou,buffer,14,46-14-i);
    lcd.setCursor(1,0);
    lcd.print(buffer);
    wait_on_escape(250);
  }
  wait_on_escape(2000);
}

