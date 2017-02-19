/*
Phi-2 shield for Arduino
Program title: interactive GPS logger version 4 

Programmed by Dr. John Liu
Revision: 6/19/2011
Free software for educational and personal uses.
No warrantee!
Commercial use without authorization is prohibited.
Find details of the Phi-1 shield, Phi-2 shield, and Phi-prompt or contact Dr. Liu at
http://liudr.wordpress.com/phi-1-shield/
http://liudr.wordpress.com/phi-2-shield/
http://liudr.wordpress.com/phi-prompt/
All rights reserved.
*/

// Menu texts
PROGMEM prog_char top_menu_item00[]="GPS display";
PROGMEM prog_char top_menu_item01[]="Erase EEPROM";
PROGMEM prog_char top_menu_item02[]="Record to EEPROM";
PROGMEM prog_char top_menu_item03[]="Send to PC";
PROGMEM prog_char top_menu_item04[]="Parameters";
PROGMEM prog_char top_menu_item05[]="Show credit";
PROGMEM const char *top_menu_items[] = {top_menu_item00, top_menu_item01, top_menu_item02, top_menu_item03, top_menu_item04, top_menu_item05};

void top_menu()
{
  int menu_pointer_1=0; // This stores the menu choice the user made.
  phi_prompt_struct myMenu; // This structure stores the main menu.

// Initialize the top menu
  myMenu.ptr.list=(char**)&top_menu_items; // Assign the list to the pointer
  myMenu.low.i=0; // Default item highlighted on the list
  myMenu.high.i=5; // Last item of the list is size of the list - 1.
  myMenu.width=lcd_columns-1; // Length in characters of the longest list item.
  myMenu.step.c_arr[0]=lcd_rows-1; // rows to auto fit entire screen
  myMenu.step.c_arr[1]=1; // one col list
  myMenu.step.c_arr[2]=0; // y for additional feature such as an index
  myMenu.step.c_arr[3]=lcd_columns-4; // x for additional feature such as an index
  myMenu.col=0; // Display prompt at column 0
  myMenu.row=1; // Display prompt at row 1
  myMenu.option=45;
  
  while(1) // This loops every time a menu item is selected.
  {
    lcd.clear();  // Refresh menu if a button has been pushed
    lcd.print("Main Menu");

    select_list(&myMenu); // Use the select_list to ask the user to select an item of the list, that is a menu item from your menu.
    menu_pointer_1=myMenu.low.i; // Get the selected item number and store it in the menu pointer.
    switch (menu_pointer_1) // See which menu item is selected and execute that correspond function
    {
      case 0:
      lcd.clear();
      top_menu_function_1();
      break;
      case 1:
      lcd.clear();
      top_menu_function_2();
      break;
      case 2:
      lcd.clear();
      top_menu_function_3();
      break;
      
      case 3:
      lcd.clear();
      top_menu_function_4();
      break;
      
      case 4:
      lcd.clear();
      top_menu_function_5();
      break;
      
      case 5:
      lcd.clear();
      top_menu_function_6();
      break;
      
      default:
      break;
    }
  }
}

void top_menu_function_1()//display
{
  recording=false;
  _record_display();
}

void top_menu_function_2()//erase
{
  unsigned long buf[4]={0,0,0,0};
  lcd.clear();
  if(yn_dialog("Erase all data?"))
  {
    lcd.clear();
    lcd.print("Erasing...");
    lcd.setCursor(0,1);
    lcd.print("Please wait");
    for (unsigned long addr=0;addr<EEPROM_size;addr+=16)
    {
      i2c_eeprom_write_page( 0x50, addr, (byte*) buf, 16);
      delay(5);
    }
  }
}

void top_menu_function_3()//record
{
  recording=true;
  _record_display();
}

void top_menu_function_4()//send_to_PC()
{
  double spdf;
  unsigned long spd;
  long lat, lon, alt;
  unsigned long age, dat, tim;
  unsigned long buf[4];
  pointer=0;
  if (!verbose) Serial.println("Lat(10^-5 deg)\tLong(10^-5 deg)\tDate(ddmmyy)\tTime(hhmmsscc)\tAlt(cm)\tSpeed(mph)");
  while (EEPROM_to_GPS(&lat, &lon, &alt, &tim, &dat, &spdf, &pointer)&&(pointer<=EEPROM_size-16))
  {
    switch (verbose)
    {
      case false:
      Serial.print(lat); Serial.print("\t"); Serial.print(lon); Serial.print("\t");
      Serial.print(dat); Serial.print("\t"); Serial.print(tim); Serial.print("\t");
      Serial.print(alt); Serial.print("\t"); printFloat(spdf); Serial.println("");
      break;
      
      case true:
      Serial.print("Lat/Long(10^-5 deg): "); Serial.print(lat); Serial.print(", "); Serial.print(lon); Serial.println("");
      Serial.print("Date(ddmmyy): "); Serial.print(dat); Serial.print(" Time(hhmmsscc): "); Serial.print(tim); Serial.println("");
      Serial.print("Alt(cm): "); Serial.print(alt); Serial.print(" Speed(mph): ");  printFloat(spdf); Serial.println(""); Serial.println("");
      break;
      
      default:
      break;
    }
  }
  pointer=0;
}

void top_menu_function_5()//parameters
{
  int user_input;
  phi_prompt_struct myIntegerInput;
  
  lcd.clear(); // Input period
  center_text("Record Period");
  lcd.setCursor(9,1);
  lcd.print("Seconds");

  user_input=period; // Current value
  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer
  myIntegerInput.low.i=1; // Lower limit
  myIntegerInput.high.i=32000; // Upper limit
  myIntegerInput.step.i=1; // Step size
  myIntegerInput.col=4; // Display prompt at column 4
  myIntegerInput.row=1; // Display prompt at row 1
  myIntegerInput.width=5; // The number occupies 5 characters space
  myIntegerInput.option=0; // Option 0, space pad right, option 1, zero pad left, option 2, space pad left.

  if (input_integer(&myIntegerInput)!=-1) period=user_input; // If the user didn't press escape (return -1) then update the ultimate storage with the value in the buffer.
  else return;

  // Input verbose mode
  lcd.clear();
  verbose=yn_dialog("Verbose mode?");

  // Input lower recording limit
  lcd.clear();
  center_text("Record from");
  lcd.setCursor(0,1);
  lcd.print("Entry#");
  user_input=lower_limit/16;
  myIntegerInput.low.i=0; // Lower limit
  myIntegerInput.high.i=EEPROM_size/16; // Upper limit
  myIntegerInput.col=7; // Display prompt at column 4

  if (input_integer(&myIntegerInput)!=-1) lower_limit=user_input*16; // If the user didn't press escape (return -1) then update the ultimate storage with the value in the buffer.
  else return;

  // Input upper recording limit
  lcd.clear();
  center_text("Record till");
  lcd.setCursor(0,1);
  lcd.print("Entry#");
  user_input=(int)(upper_limit/16UL);

  if (input_integer(&myIntegerInput)!=-1) upper_limit=((unsigned long)user_input)*16UL; // If the user didn't press escape (return -1) then update the ultimate storage with the value in the buffer.
  else return;
}

void top_menu_function_6()//credits
{
  show_credit();
}

void _record_display()
{
  double spdf;
  unsigned long spd;
  long lat, lon, alt;
  unsigned long age, dat, tim, cour;
  unsigned long buf[4];
  char msg[100]="Please wait while the GPS acquires signal";
  bool newdata = false;
  unsigned long start = millis();
  phi_prompt_struct myLongMsg;

  myLongMsg.ptr.msg=msg; // Assign the address of the text string to the pointer.
  myLongMsg.low.i=0; // Default text starting position. 0 is highly recommended.
  myLongMsg.high.i=strlen(msg); // Position of the last character in the text string, which is size of the string - 1.
  myLongMsg.step.c_arr[0]=lcd_rows; // rows to auto fit entire screen
  myLongMsg.step.c_arr[1]=lcd_columns; // one col list
  myLongMsg.col=0; // Display the text area starting at column 0
  myLongMsg.row=0; // Display the text area starting at row 0
  myLongMsg.option=0; // Option 0, display classic message, option 1, display message with scroll bar on right.

  pointer=lower_limit; // Load the lower limit.
  lcd.clear();
  long_msg_lcd(&myLongMsg);

  while(1)
  {
    byte temp1=wait_on_escape(20);
    if (feedgps()) newdata = true;
    
    switch (temp1)
    {
      case 0:
      break;
      
      case 1:
      next_line(&myLongMsg);
      long_msg_lcd(&myLongMsg);
      break;
      
      case 2:
      prev_line(&myLongMsg);
      long_msg_lcd(&myLongMsg);
      break;
      
      default:
      return;
      break;
    }

    if ((newdata)&&(millis() - start > period*1000)) // Update GPS coordinates on LCD
    {
      start=millis();
      gps.get_position(&lat, &lon, &age);
      gps.get_datetime(&dat, &tim, &age);
      alt=gps.altitude();
      spdf=gps.f_speed_mph();
      cour=gps.course();
      if (abs(spdf)>200) spdf=0.0;
      sprintf(msg,"Lat:%ld\nLong:%ld\nAlt:%ld %3d MPH\nCourse:%ld\n%02d/%02d/%02d %02d:%02d:%02d\nDr. Liu 6/19/11",lat,lon,alt, int(spdf),cour/100, (int)((dat/100)%100),(int)(dat/10000),(int)(dat%100),(int)((tim/100)/10000),(int)(((tim/100)/100)%100),(int)((tim/100)%100));
      long_msg_lcd(&myLongMsg);

      if (recording)
      {
        if (pointer<upper_limit)
        {
          GPS_to_EEPROM(&pointer);
          lcd.setCursor(lcd_columns-1,0);
          lcd.write(7);
        }
        else
        {
          lcd.clear();
          lcd.print("Limit reached");
          wait_on_escape(2000);
          return;
        }
      }
    }
  }
}
