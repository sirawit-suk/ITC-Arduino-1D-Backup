#include <SoftwareSerial.h>
#include <String.h>
#include "LedControl.h"
#include "FontLEDClock.h"
SoftwareSerial MyDATA(2, 3); //RX, TX
LedControl lc=LedControl(11,13,10,4); //Din, Clock, Csload, 4 Module

void setup() {
  
  //Arduino board
  Serial.begin(4800); //Set this suitable baud rate
  MyDATA.begin(4800); //for 4800 
  
  //Dot Matrix
  int devices=4;
  for(int address = 0; address < devices; address++) {  // set up each device 
    lc.shutdown(address,false);
    lc.setIntensity(address,6);
    lc.clearDisplay(address);
  }
}
////////////////////////////////////// Main Loop ////////////////////////////////
char input_Text[3]; // Only 3 chars
char temp_Text[3];
int Array=0;
void loop() {

  Array=0;
  while(MyDATA.available()){
    input_Text[Array]=MyDATA.read();
    Array++;
  }
                                                        //Only for 0-9
    print_Matrix(1, 1, input_Text[0]);                 //First Char
  if( input_Text[1] >= '0' && input_Text[1] <= '9'){       //Only for 0-9
    print_Matrix(9, 1, input_Text[1]);                 //Second Char
  } 
  if( input_Text[2] >= '0' && input_Text[2] <= '9'){        //Only for 0-9
    print_Matrix(17, 1, input_Text[2]);                 //Third Char
  }
  
  for(int i=0; i<3 ; i++){
    //Serial.print(input_Text[i]);     //Debug on Serial Monitor
  }

  for(int i=0; i<3; i++){
    input_Text[i] = '\0';
  }

  //delay(50);
  clear_display();
  
}

//////////////////////////////////All function of Matrixs///////////////////////////////////////////
  
//plot a point on the display
void plot (uint8_t x, uint8_t y, uint8_t value) {

  //select which matrix depending on the x coord
  uint8_t address;
  if (x >= 0 && x <= 7)   {
    address = 3;
  }
  if (x >= 8 && x <= 15)  {
    address = 2;
    x = x - 8;
  }
  if (x >= 16 && x <= 23) {
    address = 1;
    x = x - 16;
  }
  if (x >= 24 && x <= 31) {
    address = 0;
    x = x - 24;
  }

  if (value == 1) {lc.setLed(address, y, x, true);} 
  else {lc.setLed(address, y, x, false);}
}

//clear screen
void clear_display() {
  for (uint8_t address = 0; address < 4; address++) {
    lc.clearDisplay(address);
  }
}

//Libery font of MR.THANA , thanks to him :)
void print_Matrix(uint8_t x, uint8_t y, char c)
{
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) { c &= 0x1F; }  // A-Z maps to 1-26 
  else if (c >= '0' && c <= '9') { c = (c - '0') + 32; }
  else if (c == ' ') { c = 0;  } // space 
  else if (c == '.') { c = 27; } // full stop 
  else if (c == ':') { c = 28; } // colon 
  else if (c == '\''){ c = 29; } // single quote mark 
  else if (c == '!') { c = 30; } // single quote mark 
  else if (c == '?') { c = 31; } // single quote mark 

  for (uint8_t col = 0; col < 3; col++) {
    dots = pgm_read_byte_near(&mytinyfont[c][col]);
    for (uint8_t row = 0; row < 5; row++) {
      if (dots & (16 >> row))
        plot(x + col, y + row, 1);
      else
        plot(x + col, y + row, 0);
    }
  }
}
