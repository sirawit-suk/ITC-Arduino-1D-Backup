#include <SoftwareSerial.h>
#define SIZE 16
#define x_Pin A0

SoftwareSerial MyDATA(2,3); //RX, TX 

int buffer_Value[SIZE];
int buffer_Position = 0;
int x_Angle;

void setup(){
  Serial.begin(4800);  //Set this suitable baud rate
  MyDATA.begin(4800);  //for 4800 
}
void loop() { 
  int x_Axis = analogRead(x_Pin);         //Read Input x_Axis

  x_Angle = map(x_Axis,267,397,0,180);   //x_min = 267 // x_max = 398 
  
  if(x_Angle < 0){              //Case 1 below 0
    x_Angle = 0;
  }
  else if(x_Angle >180){        //Case 2 above 180
    x_Angle = 180;
  }
  
  //Serial.print("x_Angle ");   //Print at my board.
  //Serial.println(x_Angle); 
  MyDATA.println(x_Angle);      //Send print to another board.

}

int get_x()
{ 
  //delay(50);
  buffer_Value[buffer_Position] = analogRead(x_Pin);
  buffer_Position = (buffer_Position + 1) % SIZE;

  long sum = 0;
  for (unsigned int i = 0; i < SIZE; i++)
  sum += buffer_Value[i];
  
  return round(sum / SIZE);
}
