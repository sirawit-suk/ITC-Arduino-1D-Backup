#include "ST7735_TEE.h"

TEE_ST7735 lcd(9,10,11,12,13); //Arduino  csk,sda,A0,rst,cs

  //Times
float hour  = 23;
float minute = 58;
int second = 30;
int nameday = 7;
int day = 31; 
int month = 12;
int year = 99;
int day_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

int Temp_nameday = 1;
int Temp_day = 5; 
int Temp_month = 11;
int Temp_year = 62;

int Temp_second = 0;
int Temp_minute = 0;
int Temp_hour = 0;

  //stopWatch
int hour_sw  = 0;
int minute_sw = 0;
int second_sw = 0;
int Temp_hour_sw  = 0;
int Temp_minute_sw = 0;
int Temp_second_sw = 0;
int Temp2_hour_sw  = 0;
int Temp2_minute_sw = 0;
int Temp2_second_sw = 0;

int switch_Time = 0;
bool blinking = 0;
bool switchblinking = 0;

int timer_Counter;

int origin_x = 64;
int origin_y = 60;


float second_angle;
float second_x = origin_x;
float second_y = origin_y;
float Temp_second_x;
float Temp_second_y;

float minute_angle;
float minuteTemp_angle;
float minute_x = origin_x;
float minute_y = origin_y;
float Temp_minute_x;
float Temp_minute_y;

float hour_angle;
float hourTemp_angle;
float hour_x = origin_x;
float hour_y = origin_y;
float Temp_hour_x;
float Temp_hour_y;

int x = 6;
int y = 125;
int space = 17;
int spacebt = 27;
int Size = 2;
int specific = 13;
int setup_angle;
int setup_x;
int setup_y;


int date_x = 15;
int date_y = 145;
int date_sp_x = 8;
int date_spbt_x = 20;


bool mode = 0;
bool isChange = 0;
int left_button = 5; //ChangeMode

bool reset = 0;
bool resetChange = 0;
bool firsttime = 1;
int up_button = 2; //Reset

bool play = 0;
bool playChange = 0;
int right_button = 3; //Start tinking

bool setup_clock = true;
bool setup_sw = true;

unsigned int now,last1,last2,last3;

void setup()
{  
 Serial.begin(9600);
 noInterrupts();                /* disable all interrupts             */
 TCCR1A = 0;
 TCCR1B = 0;
 timer_Counter = 3036;         /* preload timer 65536-16MHz/256/1Hz  */
 TCNT1 = timer_Counter;       /* preload timer                      */
 TCCR1B |= (1 << CS12);       /* 256 prescaler                      */
 TIMSK1 |= (1 << TOIE1);     /* enable timer overflow interrupt    */
 interrupts();              /* enable all interrupts              */
 lcd.init(lcd.VERTICAL);      //TFT SetUp

 Serial.println("If you want wanna setting... Type \"setting\"");
}

/////////////////////////////////////////////// Interrupt service routine //////////////////////////////////
ISR(TIMER1_OVF_vect){                                     
  TCNT1 = timer_Counter;                // + ONLY ONE SECOND !!!!! // DO NOT CHANGE ANYTHING IN HERE
  
  Temp_second = second;
  Temp_minute = minute;
  Temp_hour = hour;

  Temp_nameday = nameday;
  Temp_day = day;
  Temp_month = month;
  Temp_year = year;

  Temp2_second_sw = Temp_second_sw;
  Temp2_minute_sw = Temp_minute_sw;
  Temp2_hour_sw = Temp_hour_sw;
  
  blinking = true;
  switchblinking = !switchblinking;     
  switch_Time++;
 
  //Norms Time
  second++; 
  if (second > 59) { second = 0; minute++;}
  if (minute > 59) { minute = 0; hour++; }
  if (hour > 23) { hour = 0; day++; nameday++;}
  if (nameday > 7) { nameday = 1;}
  
  //StopWatch Time
  second_sw++;
  if (second_sw > 59) { second_sw = 0; minute_sw++; }
  if (minute_sw > 59) { minute_sw = 0; hour_sw++; }
  if (hour_sw > 99) { hour_sw = 0; }

  if((year+1)%4 == 0){ day_month[1] = 29; } //Only for BC.
  else{ day_month[1] = 28; }    
  if (year > 99) { year = 0;}
  if (month > 12) { month = 1; year++; }
  if (day > day_month[month-1]) { day = 1; month++; }
}


void loop() {

  Input_System();
  Serial.println(month);
  if(mode == 0){
    if(setup_clock == true){
       lcd.fillScreen(BLACK);       //TFT SetColor...
       AllClock_Setup();
       setup_clock = false;
       setup_sw = true;
    }
          if(Serial.available()>0){
          if(Serial.readString() == "setting"){
             Serial.println("Please Set Hour in number only (1-23) : ");
                while(!Serial.available()>0){}
                hour = Serial.parseInt();
             Serial.println("Please Set Minute in number only (0-59): ");
                while(!Serial.available()>0){}
                minute = Serial.parseInt();
             Serial.println("Please Set Second in number only (0-59): ");
                while(!Serial.available()>0){}
                second = Serial.parseInt();
             Serial.println("Please Set Name Day in number only: ");
             Serial.println("Monday Type \"1\" : ");
             Serial.println("Tuesday Type \"2\" : ");
             Serial.println("Wednesday Type \"3\" : ");
             Serial.println("Thursday Type \"4\" : ");
             Serial.println("Friday Type \"5\" : ");
             Serial.println("Saturday Type \"6\" : ");
             Serial.println("Sunday Type \"7\" : ");
                while(!Serial.available()>0){}
                nameday = Serial.parseInt();
             Serial.println("Please Set Year(BE) in number only 2 digits: ");
                while(!Serial.available()>0){}
                year = Serial.parseInt();
             Serial.println("Please Set Month in number only (1-12): ");
                while(!Serial.available()>0){}
                month = Serial.parseInt();
             Serial.println("Please Set Day in number only (1-31): ");
                while(!Serial.available()>0){}
                day = Serial.parseInt();
                lcd.fillScreen(BLACK);
                AllClock_Setup();
          }
        }
          
         if(blinking == true)
         {
             Draw_Clock();
             tft_AnalogSecondClock();
             tft_AnalogMinuteClock();
             tft_AnalogHourClock();
             tft_DigitalClock();
             tft_DateTime();
             blinking = false;
         }
  }
  else if (mode == 1){

        if(blinking == true){
             if(setup_sw == true){
             lcd.fillScreen(RED);
             lcd.fillRect(0,origin_y-10 ,140,40, BLACK);  
             setup_sw = false;
             setup_clock = true;
             }
            lcd.Printstr(20,30,(char*)"Stop Watch",YELLOW);
            if(blinking == true){
               tft_StopWatch();
            }
            blinking = false;
        }

  }
  
}

void Input_System(){
    now = millis();
    
    isChange = digitalRead(left_button);
    if(isChange == 0 && mode == 0 && now - last1 >= 3000){
      mode = 1;
      last1 = now;
    }
    else if(isChange == 0 && mode == 1 && now - last1 >= 3000){
      mode = 0;
      last1 = now;
    }
    
    reset = digitalRead(up_button);
    playChange = digitalRead(right_button);
    if(playChange == 0 && play == 0 && now - last3 >= 1000){
      play = 1;
      last3 = now;
    }
    else if(playChange == 0 && play == 1 && now - last3 >= 1000){
      play = 0;
      last3 = now;
    }

}

void tft_AnalogSecondClock() {
  
      second_angle = map(second,0,59,-90,264);
      Temp_second_x = second_x;
      Temp_second_y = second_y;
      second_x = origin_x + (45 * cos(second_angle/(180/PI))); 
      second_y = origin_y + (45 * sin(second_angle/(180/PI)));
      lcd.drawLine(Temp_second_x, Temp_second_y, origin_x, origin_y, YELLOW);
      lcd.drawLine(second_x, second_y, origin_x, origin_y, RED);
        
}

void tft_AnalogMinuteClock() {
  
      minuteTemp_angle = map(second,0,59,0,1);
      minute_angle = map(minute+minuteTemp_angle,0,59,-90,264);
      Temp_minute_x = minute_x;
      Temp_minute_y = minute_y;
      minute_x = origin_x + (43 * cos(minute_angle/(180/PI))); 
      minute_y = origin_y + (43 * sin(minute_angle/(180/PI)));
      lcd.drawLine(Temp_minute_x, Temp_minute_y, origin_x, origin_y, YELLOW);
      lcd.drawLine(minute_x, minute_y, origin_x, origin_y, BLUE);
    
}

void tft_AnalogHourClock() {
  
      hourTemp_angle = map(minute,0,59,0,1);
      hour_angle = map(hour+hourTemp_angle,0,12,-90,270);
      Temp_hour_x = hour_x;
      Temp_hour_y = hour_y;
      hour_x = origin_x + (25 * cos(hour_angle/(180/PI))); 
      hour_y = origin_y + (25 * sin(hour_angle/(180/PI)));
      lcd.drawLine(Temp_hour_x, Temp_hour_y, origin_x, origin_y, YELLOW);
      lcd.drawLine(hour_x, hour_y, origin_x, origin_y, BLUE);
    
}

void tft_DigitalClock() {
  
  //Delete print
      if(Temp_hour/10 != (int)hour/10){
        lcd.drawChar(x,y,Temp_hour /10 + '0', BLACK, Size);
      }
      if(Temp_hour%10 != (int)hour%10){
        lcd.drawChar(x+space,y,Temp_hour %10 + '0', BLACK, Size);
      }
   //Minite
      if(Temp_minute/10 != (int)minute/10){
        lcd.drawChar(x+space*1+spacebt,y,Temp_minute /10 + '0', BLACK, Size);
      }
      if(Temp_minute%10 != (int)minute%10){
        lcd.drawChar(x+space*2+spacebt,y,Temp_minute %10 + '0', BLACK, Size);
      }
   //Second
      if(Temp_second/10 != second/10){
        lcd.drawChar(x+space*2+spacebt*2,y,Temp_second /10 + '0', BLACK, Size);
      }
        lcd.drawChar(x+space*3+spacebt*2,y,Temp_second %10 + '0', BLACK, Size);
  
    //Normal print 
     lcd.drawChar(x,y,hour /10 + '0', RED, Size);
     lcd.drawChar(x+space,y,(int)hour %10 + '0', YELLOW, Size);
     
     lcd.drawChar(x+space+spacebt,y, minute /10 + '0', GREEN, Size);
     lcd.drawChar(x+space*2+spacebt,y,(int)minute %10 + '0',WHITE, Size);
     
     lcd.drawChar(x+space*2+spacebt*2,y,second /10 + '0', CYAN, Size);
     lcd.drawChar(x+space*3+spacebt*2,y,second %10 + '0', MAGENTA, Size);
  
     Draw_Blink();
}

void tft_DateTime(){

       //Date Name
       if(nameday == 1){
        if(Temp_nameday != 1){
          lcd.Printstr(date_x,date_y-5,(char*)"SUN",BLACK);
        }
          lcd.Printstr(date_x,date_y-5,(char*)"MON",WHITE);
       }
       else if(nameday == 2){
        if(Temp_nameday != 2){
          lcd.Printstr(date_x,date_y-5,(char*)"MON",BLACK);
        }
          lcd.Printstr(date_x,date_y-5,(char*)"TUE",WHITE);
       }
       else if(nameday == 3){
        if(Temp_nameday != 3){
          lcd.Printstr(date_x,date_y-5,(char*)"TUE",BLACK);
        }
          lcd.Printstr(date_x,date_y-5,(char*)"WED",WHITE);
       }
       else if(nameday == 4){
        if(Temp_nameday != 4){
          lcd.Printstr(date_x,date_y-5,(char*)"WED",BLACK);
        }
          lcd.Printstr(date_x,date_y-5,(char*)"THU",WHITE);
       }
       else if(nameday == 5){
        if(Temp_nameday != 5){
          lcd.Printstr(date_x,date_y-5,(char*)"THU",BLACK);
        }
          lcd.Printstr(date_x,date_y-5,(char*)"FRI",WHITE);
       }
       else if(nameday == 6){
        if(Temp_nameday != 6){
          lcd.Printstr(date_x,date_y-5,(char*)"FRI",BLACK); 
        }
          lcd.Printstr(date_x,date_y-5,(char*)"SAT",WHITE);
       }
       else if(nameday == 7){
        if(Temp_nameday != 7){
          lcd.Printstr(date_x,date_y-5,(char*)"SAT",BLACK);
        }
          lcd.Printstr(date_x,date_y-5,(char*)"SUN",WHITE);
       }

      if(Temp_day/10 != day/10){
        lcd.drawChar(date_x+22 + date_spbt_x, date_y, Temp_day /10 + '0', BLACK, 1);  
      }
      if(Temp_day%10 != day%10){
        lcd.drawChar(date_x+22 + date_spbt_x + date_sp_x, date_y, Temp_day %10 + '0', BLACK, 1);
      }
      if(Temp_month/10 != month/10){
        lcd.drawChar(date_x+22 + date_spbt_x*2, date_y, Temp_month /10 + '0', BLACK, 1);
      }
      if(Temp_month%10 != month%10){
        lcd.drawChar(date_x+22 + date_spbt_x*2 + date_sp_x, date_y, Temp_month %10 + '0', BLACK, 1);
      }
      if(Temp_year/10 != year/10){
        lcd.drawChar(date_x+22 + date_spbt_x*3, date_y, Temp_year /10 + '0', BLACK, 1);
      }
      if(Temp_year%10 != year%10){
        lcd.drawChar(date_x+22 + date_spbt_x*3 + date_sp_x, date_y, Temp_year %10 + '0', BLACK, 1);
      }
      
      lcd.drawChar(date_x+22 + date_spbt_x, date_y,day /10 + '0', CYAN, 1);
      lcd.drawChar(date_x+22 + date_spbt_x + date_sp_x, date_y,day %10 + '0', CYAN, 1);

      lcd.drawChar(date_x+22 + date_spbt_x*2, date_y,month /10 + '0', MAGENTA, 1);
      lcd.drawChar(date_x+22 + date_spbt_x*2 + date_sp_x, date_y,month %10 + '0', MAGENTA, 1);

      lcd.drawChar(date_x+22 + date_spbt_x*3, date_y,year /10 + '0', CYAN, 1);
      lcd.drawChar(date_x+22 + date_spbt_x*3 + date_sp_x, date_y,year %10 + '0', CYAN, 1);
      
}

void tft_StopWatch(){

      if(reset == 0){
        SetInitial();
        firsttime = true;
        reset = 1;
      }
      if(play == true){
        if(firsttime == true){
          SetTemptoNorm();
          firsttime = false;
        }
        SetNormtoTemp();
      }
      else if(play == false){
        firsttime = true;
      }

   //Delete print
      if(Temp2_hour_sw/10 != Temp_hour_sw/10){
        lcd.drawChar(x,origin_y, Temp2_hour_sw /10 + '0', BLACK, Size);
      }
      if(Temp2_hour_sw%10 != Temp_hour_sw%10){
        lcd.drawChar(x+space,origin_y, Temp2_hour_sw %10 + '0', BLACK, Size);
      }
   //Minite
      if(Temp2_minute_sw/10 != Temp_minute_sw/10){
        lcd.drawChar(x+space*1+spacebt,origin_y, Temp2_minute_sw /10 + '0', BLACK, Size);
      }
      if(Temp2_minute_sw%10 != Temp_minute_sw%10){
        lcd.drawChar(x+space*2+spacebt,origin_y, Temp2_minute_sw %10 + '0', BLACK, Size);
      }
   //Second
      if(Temp2_second_sw/10 != Temp_second_sw/10){
        lcd.drawChar(x+space*2+spacebt*2,origin_y, Temp2_second_sw /10 + '0', BLACK, Size);
      }
      if(Temp2_second_sw%10 != Temp_second_sw%10){
        lcd.drawChar(x+space*3+spacebt*2,origin_y, Temp2_second_sw %10 + '0', BLACK, Size);
      }
    //Normal print 
     lcd.drawChar(x, origin_y, Temp_hour_sw /10 + '0', WHITE, Size);
     lcd.drawChar(x+space, origin_y, Temp_hour_sw %10 + '0', WHITE, Size);
     
     lcd.drawChar(x+space+spacebt, origin_y, Temp_minute_sw /10 + '0', WHITE, Size);
     lcd.drawChar(x+space*2+spacebt, origin_y, Temp_minute_sw %10 + '0', WHITE, Size);
      
     lcd.drawChar(x+space*2+spacebt*2, origin_y, Temp_second_sw /10 + '0', WHITE, Size);
     lcd.drawChar(x+space*3+spacebt*2, origin_y, Temp_second_sw %10 + '0', WHITE, Size);
  
     Draw_Blink2();
}

void SetInitial(){
    Temp_hour_sw = 0;
    Temp_minute_sw = 0;
    Temp_second_sw = 0;
}

void SetTemptoNorm(){
    hour_sw = Temp_hour_sw;
    minute_sw = Temp_minute_sw;
    second_sw = Temp_second_sw;
}

void SetNormtoTemp(){
    Temp_hour_sw = hour_sw;
    Temp_minute_sw = minute_sw;
    Temp_second_sw = second_sw;
}

void Draw_Blink(){
  
    if(switchblinking == true){
        lcd.drawChar(x+space+specific,y,':', YELLOW, Size);
        lcd.drawChar(x+space*2+spacebt+specific,y,':', YELLOW, Size);
    }
    else{
        lcd.drawChar(x+space+specific,y,':', BLACK, Size);
        lcd.drawChar(x+space*2+spacebt+specific,y,':', BLACK, Size);
    }

}

void Draw_Blink2(){
    if(switchblinking == true){
        lcd.drawChar(x+space+specific, origin_y,':', YELLOW, Size);
        lcd.drawChar(x+space*2+spacebt+specific, origin_y,':', YELLOW, Size);
    }
    else{
        lcd.drawChar(x+space+specific, origin_y,':', BLACK, Size);
        lcd.drawChar(x+space*2+spacebt+specific, origin_y,':', BLACK, Size);
    }    
}

void AllClock_Setup(){

       lcd.fillCircle(origin_x+45,origin_y-40,15, YELLOW);  
       lcd.fillCircle(origin_x-45,origin_y-40,15, YELLOW);  
       lcd.drawCircle(origin_x+45,origin_y-40,15, RED);  
       lcd.drawCircle(origin_x-45,origin_y-40,15, RED);
       lcd.drawCircle(origin_x+45,origin_y-40,17, RED);  
       lcd.drawCircle(origin_x-45,origin_y-40,17, RED);  
   
       lcd.fillCircle(origin_x,origin_y,50, YELLOW);   
        for(int i=0; i<60; i++){
          setup_angle = map(i,0,59,-90,264);
          setup_x = origin_x + (50 * cos(setup_angle/(180/PI))); 
          setup_y = origin_y + (50 * sin(setup_angle/(180/PI)));
          lcd.drawLine(setup_x, setup_y, origin_x, origin_y, MAGENTA);
        }
        
        for(int i=0; i<360; i++){
          setup_angle = i;
          setup_x = origin_x + (45 * cos(setup_angle/(180/PI))); 
          setup_y = origin_y + (45 * sin(setup_angle/(180/PI)));
          lcd.drawLine(setup_x, setup_y, origin_x, origin_y, YELLOW);
        }
       lcd.drawCircle(origin_x,origin_y,50, RED);
       lcd.drawCircle(origin_x,origin_y,51, RED);
       lcd.drawCircle(origin_x,origin_y,57, RED);
       lcd.drawCircle(origin_x,origin_y,58, RED);
       lcd.drawCircle(origin_x,origin_y,59, RED);
       
}

void Draw_Clock(){

     int plot =30;

     lcd.drawChar(100, origin_y-6, 3 + '0', MAGENTA, 2); //3
     lcd.drawChar(origin_x-5, 93, 6 + '0', MAGENTA, 2);  //6
     
     lcd.drawChar(19, origin_y-5, 9 + '0', MAGENTA, 2);  //9
     lcd.drawChar(origin_x-12, 15, 1 + '0', MAGENTA, 2);
     lcd.drawChar(origin_x,15, 2 + '0', MAGENTA, 2);  //12

     lcd.drawChar(origin_x+plot-10, origin_y-plot-10, 1 + '0', MAGENTA, 1); //1
     lcd.drawChar(origin_x+plot+5, origin_y-plot+5, 2 + '0', MAGENTA, 1); //2

     lcd.drawChar(origin_x+plot+5, origin_y+plot-11, 4 + '0', MAGENTA, 1); //4
     lcd.drawChar(origin_x+plot-10, origin_y+plot+3, 5 + '0', MAGENTA, 1); //5

     lcd.drawChar(origin_x-plot+5, origin_y+plot+3, 7 + '0', MAGENTA, 1); //7
     lcd.drawChar(origin_x-plot-7, origin_y+plot-10, 8 + '0', MAGENTA, 1); //8

     lcd.drawChar(origin_x-plot-11, origin_y-plot+5, 1 + '0', MAGENTA, 1); 
     lcd.drawChar(origin_x-plot-6, origin_y-plot+5, 0 + '0', MAGENTA, 1); //10
     
     lcd.drawChar(origin_x-plot+2, origin_y-plot-10, 1 + '0', MAGENTA, 1); 
     lcd.drawChar(origin_x-plot+7, origin_y-plot-10, 1 + '0', MAGENTA, 1); //11
}
