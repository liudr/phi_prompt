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

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint; 
  } 
}

bool feedgps()
{
  while (nss.available())
  {
    if (gps.encode(nss.read()))
      return true;
  }
  return false;
}

void GPS_to_EEPROM(unsigned long *pointer)
/*
The function assumes that the TinyGPS object gps is already initialized and ready to send data.
It also assumes that the caller feeds the GPS and checks the pointer so the pointed address will not exceed the address space of the EEPROM, or cross page boundaries while writing.
*/
{
  double spdf;
  unsigned long spd;
  long lat, lon, alt;
  unsigned long age, dat, tim;
  unsigned long buf[4];
  
  gps.get_position(&lat, &lon, &age);
  gps.get_datetime(&dat, &tim, &age);
  alt=gps.altitude();
  spdf=gps.f_speed_mph();

  spd=spdf*100;
  buf[0]=lat;
  buf[1]=lon;
  buf[2]=((alt/10)<<16)|((dat/10000)<<11)|(((dat%10000)/100)<<7)|(dat%100); // Altitude only takes 16 bit. It is in the unit of 0.1m instead of 1cm, which is not necessary in accuracy. date(when expressed in binary 5bit date-4bit month-7bit year, takes no more than 16 bit. Combine them together.
  buf[3]=(spd<<17)|((tim/1000000)*3600+((tim%1000000)/10000)*60+(tim%10000)/100); // Speed, expressed in 1/100 mph, takes less than 15 bits. Compact hhmmsscc into seconds since 00:00 and lose the 1/100 seconds. This is 17 bit long.
  i2c_eeprom_write_page(0x50, (unsigned int) (*pointer), (byte*) buf, 16); // Store 16 bytes of data at location of the pointer.
  (*pointer)=(*pointer)+16; // Increment pointer.
  delay(5); // Make sure the data is written to the EEPROM.
}

boolean EEPROM_to_GPS(long *lat, long *lon, long *alt, unsigned long *tim, unsigned long *dat, double *spdf, unsigned long *pointer)
/*
This function reads one EEPROM entry, if it's non zero, parse it into long integer forms (double precision for speed), and returns true.
If the EEPROM entry is empty, return false.
To stay isolated from the main program, this function doesn't check if the pointer will be beyond the EEPROM size, which is left to the caller to do.
*/
{
  unsigned long buf[4];
  i2c_eeprom_read_buffer (0x50, (unsigned int) (*pointer), (byte*) buf, 16);
  if ((buf[0]==0)&&(buf[1]==0)&&(buf[2]==0)&&(buf[3]==0)) return false;
  *lat=(long)buf[0];
  *lon=(long)buf[1];
  *dat=buf[2]&0xFFFF;
  *dat=(*dat>>11)*10000+((*dat>>7)&15)*100+(*dat&127); //Process data to turn into 12/27/10 form
  *alt=10*((long)buf[2]>>16);
  *tim=(buf[3]&0x1FFFF);
  *tim=(*tim/3600)*1000000+((*tim%3600)/60)*10000+(*tim%60)*100; //Process time to turn into 12:59:0000 form
  *spdf=((double)((buf[3])>>17))/100.0;
  (*pointer)=(*pointer)+16; // Increment pointer by 16 bytes.
  return true;
}

