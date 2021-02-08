#include <LedControl.h>
#include "FontLEDClock.h"

#define SW_MODE_PIN 2
#define SW_MINUS_PIN 3
#define SW_PLUS_PIN 4
#define LDR_PIN 0
#define led 8 
#define spk 7

#define SIZE 16

  //LED MATRIX 
LedControl lc = LedControl(11, 13, 10, 4); // Din , Clk, Cs, modules
int intensity = 8;

  //Times
int hour  = 1;
int minute = 48;
int second = 0;
int day = 30; 
int month = 10;
int year = 62;
int day_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

  //stopWatch
int stopWatch_hour  = 0;
int stopWatch_minute = 0;
int stopWatch_second = 0;
int stopWatchTemp_hour  = 0;
int stopWatchTemp_minute = 0;
int stopWatchTemp_second = 0;
bool isPlay_StopWatch = 0;
bool initial_StopWatch = 1;
  
  //Timers
int timer_hour  = 0;
int timer_minute = 0;
int timer_second = 0;
int timerTemp_hour  = 0;
int timerTemp_minute = 0;
int timerTemp_second = 0;
bool isPlay_Timer = 1;
bool initial_Timer = 1;

  //set Alarm
int set_Alarm_hour  = 0;
int set_Alarm_minute = 0;
int set_Alarm_second = 0;
bool set_Alarm = 0;

int switch_Time = 0;
bool blinking = 0;
 
int buffer_Value[SIZE];
int buffer_Position = 0;

int timer_Counter;

  //Buttons
int switch_Mode;
int switch_Plus;
int switch_Minus;

  //Debounce buttons
unsigned int now;
unsigned int lastblink = 0;
unsigned int specific_last1 = 0;
unsigned int last2 = 0;
unsigned int last3 = 0;
int oldstate_Mode = 1;
int state_Mode = 1;
bool isPress1 = 0;
bool isPress2 = 0;
bool isPress3 = 0;
bool state_Plus = 0;
bool state_Minus = 0;
bool isChangeMode = 0;

//////////////////////////////////////////////////////// SetUp //////////////////////////////////////////
void setup(){
  //Matrix SetUp
  int devices = 4;                   
  for (int address = 0; address < devices; address++){ 
      lc.shutdown(address, false);                      
      lc.setIntensity(address, intensity);                     
      lc.clearDisplay(address);                                  
  }

  //Arduino SetUp
  Serial.begin(9600);
  pinMode(SW_MODE_PIN,INPUT);
  pinMode(SW_PLUS_PIN,INPUT);
  pinMode(SW_MINUS_PIN,INPUT);
  pinMode(led,OUTPUT);
  pinMode(spk,OUTPUT);
  
  noInterrupts();                /* disable all interrupts             */
  TCCR1A = 0;
  TCCR1B = 0;
  timer_Counter = 3036;         /* preload timer 65536-16MHz/256/1Hz  */
  TCNT1 = timer_Counter;       /* preload timer                      */
  TCCR1B |= (1 << CS12);       /* 256 prescaler                      */
  TIMSK1 |= (1 << TOIE1);     /* enable timer overflow interrupt    */
  interrupts();              /* enable all interrupts              */
    tone(spk,2000); delay(200);
    noTone(spk); delay(200);
    tone(spk,2000); delay(200);
    noTone(spk); delay(200);
}

/////////////////////////////////////////////// Interrupt service routine //////////////////////////////////
ISR(TIMER1_OVF_vect){                                     
  TCNT1 = timer_Counter;                // + ONLY ONE SECOND !!!!! // DO NOT CHANGE ANYTHING IN HERE
  blinking = !blinking;     
  switch_Time++;

  //Norms Time
  second++; 
  if (second > 59) { second = 0; minute++; }
  if (minute > 59) { minute = 0; hour++; }
  if (hour > 23) { hour = 0; day++; }
  
  //StopWatch Time
  stopWatch_second++;
  if (stopWatch_second > 59) { stopWatch_second = 0; stopWatch_minute++; }
  if (stopWatch_minute > 59) { stopWatch_minute = 0; stopWatch_hour++; }
  if (stopWatch_hour > 99) { stopWatch_hour = 0; }
  
  //Timer Time
  timer_second--;
  if (timer_second < 0 && timer_minute == 0 && timer_hour == 0) { timer_second = 0;}
  else if (timer_second < 0 ) {timer_second = 59; timer_minute--;}
  if (timer_minute < 0 && timer_hour == 0) { timer_minute = 0;}
  else if (timer_minute < 0 ) {timer_minute = 59; timer_hour--;}
  if (timer_hour < 0) { timer_hour = 0; }
  
  if (day > day_month[month-1]) { day = 1; month++; }
  if (month > 12) { month = 1; year++; }
  if (year > 99) { year = 0;}
  if((year+1)%4 == 0){ day_month[1] = 29; } //Only for BC.                
  else{ day_month[1] = 28; }
}

/////////////////////////////////////////////////// Main Loop /////////////////////////////////////////////
void loop() {
  Input();
  Change_Intensity();
  Change_Mode();
  Check_Alarm();
}
/////////////////////////////////////////////////// Input Functions /////////////////////////////////////////////
void Input()
{
  if(state_Minus || state_Plus){
      tone(spk,2000); delay(100);
      noTone(spk); delay(1);
  }
  if(isChangeMode){
      tone(spk,2500); delay(500);
      noTone(spk); delay(1);
  }
    now = millis();
  //Input Press buttons
    switch_Mode = digitalRead(SW_MODE_PIN);
    switch_Minus = digitalRead(SW_MINUS_PIN);
    switch_Plus = digitalRead(SW_PLUS_PIN); 
  //Debounce buttons
    //Mode
      if(switch_Mode == 1 && isPress1 == 0){
        tone(spk,2000); delay(100);
        noTone(spk); delay(1);    
        state_Mode++;
        isPress1 = 1;
        specific_last1 = now;
      }
      else if(switch_Mode == 0 && isPress1 == 1){
        isPress1 = 0;
      }
      if(isPress1 == 1 && now - specific_last1 >= 2000){
        isChangeMode = 1;
        specific_last1 = now;
      }
      else{
        isChangeMode = 0;
      }
    //Minus      
      if(switch_Minus == 1 && isPress2 == 0){                       
        state_Minus = 1;
        isPress2 = 1;
      }
      else if (isPress2 == 1 && now - last2 >= 200){
        state_Minus = 0;
        last2 = now;
      }
      else {
        isPress2 = 0;
      }
   //Plus 
      if(switch_Plus == 1 && isPress3 == 0){                       
        state_Plus = 1;
        isPress3 = 1;
      }
      else if (isPress3 == 1 && now - last3 >= 200){
        state_Plus = 0;
        last3 = now;
      }
      else {
        isPress3 = 0;
      }

}
/////////////////////////////////////////////////// Change Mode Functions /////////////////////////////////////////////
void Change_Mode(){
  if(state_Mode == 1) {Change_Screen(); state_Mode++;}                              //All Main Mode
  else if(state_Mode == 2) {Main_Mode();} 
  
  else if(state_Mode == 3) {Change_Screen(); state_Mode++;}
  else if(state_Mode == 4) {Draw_Alarm(); delay(500); state_Mode++;}
  else if(state_Mode == 5) {Clear_Display(); state_Mode++;}
  else if(state_Mode == 6) {Set_On_Off();}
  else if(state_Mode == 7) {Clear_Display(); state_Mode++;}
  else if(state_Mode == 8) {Set_Alarm_First();}
  else if(state_Mode == 9) {Set_Alarm_Second();}
  else if(state_Mode == 10) {Set_Alarm_Third();}
  
  else if(state_Mode == 11) {Change_Screen(); state_Mode++;}
  else if(state_Mode == 12) {Draw_StopWatch(); delay(500); state_Mode++;}
  else if(state_Mode == 13) {Clear_Display(); state_Mode++;}
  else if(state_Mode == 14) {StopWatch();} 
  
  else if(state_Mode == 15) {Change_Screen(); state_Mode++;}
  else if(state_Mode == 16) {Draw_Timer(); delay(500); state_Mode++;}
  else if(state_Mode == 17) {Clear_Display(); state_Mode++;}
  else if(state_Mode == 18) {Set_Timer_First();}
  else if(state_Mode == 19) {Set_Timer_Second();}
  else if(state_Mode == 20) {Set_Timer_Third(); initial_Timer = 1;}
  else if(state_Mode == 21) {Timer();}
  else if(state_Mode == 22) {state_Mode = 1;}

  if((state_Mode >=1 && state_Mode <= 22) && isChangeMode == 1) {Change_Screen(); state_Mode = 23;}       //SetTimes Mode
  else if(state_Mode == 23) {Draw_SetTime(); delay(500); state_Mode++;}
  else if(state_Mode == 24) {Clear_Display(); state_Mode++;}
  else if(state_Mode == 25) {Set_Time_Hour();}
  else if(state_Mode == 26) {Set_Time_Minute();}
  else if(state_Mode == 27) {Set_Time_Second();}
  else if(state_Mode == 28) {Change_Screen(); state_Mode++;}
  else if(state_Mode == 29) {Draw_SetDMY(); delay(500); state_Mode++;}
  else if(state_Mode == 30) {Clear_Display(); state_Mode++;}
  else if(state_Mode == 31) {Set_Day();}
  else if(state_Mode == 32) {Set_Month();}
  else if(state_Mode == 33) {Set_Year();}
  else if(state_Mode == 34) {state_Mode = 1;}

}
/////////////////////////////////////////////////// All Mode Functions /////////////////////////////////////////////
void Main_Mode(){
  //Time & Day
    if(switch_Time < 7){ Main_Time(); }
    else if (switch_Time >= 7){ Main_Day(); }

    while(switch_Time == 7 || switch_Time >= 14){
      if(switch_Time >= 14) {switch_Time = 0;}
      Change_Screen();
    }
}
void Main_Time(){
        Draw_Blink();
   //Print Time...
        print_tiny_char( 1, 2,  hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  hour % 10 + '0'); //print hour nd.
        print_tiny_char(13, 2, minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, minute % 10 + '0'); //print minute nd.
        print_tiny_char(25, 2, second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, second % 10 + '0'); //print second nd.
}
void Main_Day(){
        Draw_Slash();
  //Print Day...
        print_tiny_char( 1, 2,  day / 10 + '0'); //print day st.
        print_tiny_char( 5, 2,  day % 10 + '0'); //print day nd.
        print_tiny_char(13, 2, month / 10 + '0'); //print month st.
        print_tiny_char(17, 2, month % 10 + '0'); //print month nd.
        print_tiny_char(25, 2, year / 10 + '0'); //print year st.
        print_tiny_char(29, 2, year % 10 + '0'); //print year nd.
  
}
/////////////////////////////////////////////////// Alarm /////////////////////////////////////////
void Check_Alarm(){
  if(set_Alarm == 1) {
    digitalWrite(led,HIGH);
    if(set_Alarm_hour == hour && set_Alarm_minute == minute && set_Alarm_second == second){
      Change_Screen();
        for(int i = 0; i < 100; i++){
          if(i%2 == 0){
            Draw_Wakeup();
          }
          else{
            Clear_Display();
          }
          tone(spk,2500); delay(100);
          noTone(spk); delay(100);
          if(i%10 == 0){
            delay(1000);
          }
        }
      Change_Screen();       
    }
  }
  else{
    digitalWrite(led,LOW);
  }
  
}
void Set_On_Off(){
  Draw_OnOff();
  if(state_Minus == 1) {set_Alarm = 1;}
  if(state_Plus == 1) {set_Alarm = 0;}
}
void Set_Alarm_First(){
    if (set_Alarm_hour > 23) { set_Alarm_hour = 0; }
    else if (set_Alarm_hour < 0) { set_Alarm_hour = 23;}
    if(state_Plus == 1) {set_Alarm_hour++;}
    if(state_Minus == 1) {set_Alarm_hour--;}

    //Make it blink to Set new value
    if(now - lastblink >= 300){
        print_tiny_char( 1, 2,  set_Alarm_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  set_Alarm_hour % 10 + '0'); //print hour nd.
    }
    if (now - lastblink >= 600){
        Clear_First();
        lastblink = now;
    }
        Draw_Blink();
   //Print Time...
        print_tiny_char(13, 2, set_Alarm_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, set_Alarm_minute % 10 + '0'); //print minute nd.
        print_tiny_char(25, 2, set_Alarm_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, set_Alarm_second % 10 + '0'); //print second nd.
}
void Set_Alarm_Second(){
    if (set_Alarm_minute > 59) { set_Alarm_minute = 0; }
    else if (set_Alarm_minute < 0) { set_Alarm_minute = 59;}
    if(state_Plus == 1) {set_Alarm_minute++;}
    if(state_Minus == 1) {set_Alarm_minute--;}

    //Make it blink to Set new value
    if(now - lastblink >= 300){
        print_tiny_char(13, 2, set_Alarm_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, set_Alarm_minute % 10 + '0'); //print minute nd.
    }
    if (now - lastblink >= 600){
        Clear_Second();
        lastblink = now;
    }
        Draw_Blink();
   //Print Time...
        print_tiny_char( 1, 2,  set_Alarm_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  set_Alarm_hour % 10 + '0'); //print hour nd.
        print_tiny_char(25, 2, set_Alarm_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, set_Alarm_second % 10 + '0'); //print second nd.
}
void Set_Alarm_Third(){
  if (set_Alarm_second > 59) { set_Alarm_second = 0; }
    else if (set_Alarm_second < 0) { set_Alarm_second = 59;}
    if(state_Plus == 1) {set_Alarm_second++;}
    if(state_Minus == 1) {set_Alarm_second--;}

    //Make it blink to Set new value
    if(now - lastblink >= 300){
        print_tiny_char(25, 2, set_Alarm_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, set_Alarm_second % 10 + '0'); //print second nd.
    }
    if (now - lastblink >= 600){
        Clear_Third();
        lastblink = now;
    }
        Draw_Blink();
   //Print Time...
        print_tiny_char( 1, 2,  set_Alarm_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  set_Alarm_hour % 10 + '0'); //print hour nd.
        print_tiny_char(13, 2, set_Alarm_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, set_Alarm_minute % 10 + '0'); //print minute nd.
}
/////////////////////////////////////////////////// StopWatch /////////////////////////////////////////
void StopWatch(){
  if(state_Minus == 1) {        //Play and Pause
    if(isPlay_StopWatch == 0){
      isPlay_StopWatch = 1;
    }
    else if(isPlay_StopWatch == 1){
      isPlay_StopWatch = 0;
    }
  }
  if(state_Plus == 1) {         //Reset
      Set_StopWatch_Zero();
      initial_StopWatch = 1;
      isPlay_StopWatch = 0;
  }

  //Functions of playing
  if(isPlay_StopWatch == 0){
    if(initial_StopWatch == 1){
      Set_StopWatch_Zero();
      initial_StopWatch = 0;  
    }
    else if (initial_StopWatch == 0){
      Set_StopWatch_TempToNorm();
    }
  }
  else if (isPlay_StopWatch == 1){
      Set_StopWatch_NormToTemp();
  }
        Draw_Blink();
   //Print Time...
        print_tiny_char( 1, 2,  stopWatchTemp_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  stopWatchTemp_hour % 10 + '0'); //print hour nd.
        print_tiny_char(13, 2, stopWatchTemp_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, stopWatchTemp_minute % 10 + '0'); //print minute nd.
        print_tiny_char(25, 2, stopWatchTemp_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, stopWatchTemp_second % 10 + '0'); //print second nd.

}
/////////////////////////////////////////////////// Timer /////////////////////////////////////////
void Timer(){
  if(state_Minus == 1) {        //Play and Pause
    if(isPlay_Timer == 1){
      isPlay_Timer = 0;
    }
    else if(isPlay_Timer == 0){
      isPlay_Timer = 1;
    }
  }
  if(state_Plus == 1) {         //Reset
      Set_Timer_Zero();
      initial_Timer = 1;
      isPlay_Timer = 1;
      state_Mode = 18;
  }

  //Functions of playing
  if(isPlay_Timer == 1){
    if(initial_Timer == 1){
      Set_Timer_TempToNorm();
      initial_Timer = 0;  
    }
    else if (initial_Timer == 0){
      Set_Timer_NormToTemp();
    }
  }
  else if (isPlay_Timer == 0){
    Set_Timer_TempToNorm();
  }
        Draw_Blink();
   //Print Time...
        print_tiny_char( 1, 2,  timerTemp_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  timerTemp_hour % 10 + '0'); //print hour nd.
        print_tiny_char(13, 2, timerTemp_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, timerTemp_minute % 10 + '0'); //print minute nd.
        print_tiny_char(25, 2, timerTemp_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, timerTemp_second % 10 + '0'); //print second nd.
}
/////////////////////////////////////////////////// Small Timer Functions ///////////////////////////////////////
void Set_Timer_First(){
  if(timerTemp_hour < 0) {timerTemp_hour = 99;}
  else if(timerTemp_hour > 99) {timerTemp_hour = 0;}
  if(state_Plus == 1) {timerTemp_hour++;}
  if(state_Minus == 1) {timerTemp_hour--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
      print_tiny_char( 1, 2,  timerTemp_hour / 10 + '0'); //print hour st.
      print_tiny_char( 5, 2,  timerTemp_hour % 10 + '0'); //print hour nd.
  }
  if (now - lastblink >= 600){
      Clear_First();
      lastblink = now;
  }
        Draw_Blink();
  //Print the rest of it
        print_tiny_char(13, 2, timerTemp_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, timerTemp_minute % 10 + '0'); //print minute nd.
        print_tiny_char(25, 2, timerTemp_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, timerTemp_second % 10 + '0'); //print second nd.
}
void Set_Timer_Second(){
  if(timerTemp_minute < 0) {timerTemp_minute = 59;}
  else if(timerTemp_minute > 59) {timerTemp_minute = 0;}
  if(state_Plus == 1) {timerTemp_minute++;}
  if(state_Minus == 1) {timerTemp_minute--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
        print_tiny_char(13, 2, timerTemp_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, timerTemp_minute % 10 + '0'); //print minute nd.
  }
  if (now - lastblink >= 600){
      Clear_Second();
      lastblink = now;
  }
        Draw_Blink();
  //Print the rest of it
        print_tiny_char( 1, 2,  timerTemp_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  timerTemp_hour % 10 + '0'); //print hour nd.
        print_tiny_char(25, 2, timerTemp_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, timerTemp_second % 10 + '0'); //print second nd.
}
void Set_Timer_Third(){
  if(timerTemp_second < 0) {timerTemp_second = 59;}
  else if(timerTemp_second > 59) {timerTemp_second = 0;}
  if(state_Plus == 1) {timerTemp_second++;}
  if(state_Minus == 1) {timerTemp_second--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
        print_tiny_char(25, 2, timerTemp_second / 10 + '0'); //print second st.
        print_tiny_char(29, 2, timerTemp_second % 10 + '0'); //print second nd.
  }
  if (now - lastblink >= 600){
      Clear_Third();
      lastblink = now;
  }
        Draw_Blink();
  //Print the rest of it
        print_tiny_char( 1, 2,  timerTemp_hour / 10 + '0'); //print hour st.
        print_tiny_char( 5, 2,  timerTemp_hour % 10 + '0'); //print hour nd.
        print_tiny_char(13, 2, timerTemp_minute / 10 + '0'); //print minute st.
        print_tiny_char(17, 2, timerTemp_minute % 10 + '0'); //print minute nd.
}
/////////////////////////////////////////////////// Small Functions /////////////////////////////////////////////
void Set_StopWatch_Zero(){
      stopWatchTemp_second = 0;
      stopWatchTemp_minute = 0;
      stopWatchTemp_hour = 0;
      stopWatch_second = 0;
      stopWatch_minute = 0;
      stopWatch_hour = 0;
}
void Set_StopWatch_NormToTemp(){
      stopWatchTemp_second = stopWatch_second;
      stopWatchTemp_minute = stopWatch_minute;
      stopWatchTemp_hour = stopWatch_hour;
}
void Set_StopWatch_TempToNorm(){
      stopWatch_second = stopWatchTemp_second;
      stopWatch_minute = stopWatchTemp_minute; 
      stopWatch_hour = stopWatchTemp_hour;
}
void Set_Timer_Zero(){
      timerTemp_second = 0;
      timerTemp_minute = 0;
      timerTemp_hour = 0;
      timer_second = 0;
      timer_minute = 0;
      timer_hour = 0;
}
void Set_Timer_NormToTemp(){
      timerTemp_second = timer_second;
      timerTemp_minute = timer_minute;  
      timerTemp_hour = timer_hour;
}
void Set_Timer_TempToNorm(){
      timer_second = timerTemp_second;
      timer_minute = timerTemp_minute; 
      timer_hour = timerTemp_hour;
}
/////////////////////////////////////////////////// Set Time Mode /////////////////////////////////////////////
void Set_Time_Hour(){
  if(hour < 0) {hour = 23;}
  else if(hour > 23) {hour = 0;}
  if(state_Plus == 1) {hour++;}
  if(state_Minus == 1) {hour--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
      print_tiny_char( 1, 2,  hour / 10 + '0'); //print hour st.
      print_tiny_char( 5, 2,  hour % 10 + '0'); //print hour nd.
  }
  if (now - lastblink >= 600){
      Clear_First();
      lastblink = now;
  }
      Draw_Blink();
  //Print the rest of it
      print_tiny_char(13, 2, minute / 10 + '0'); //print minute st.
      print_tiny_char(17, 2, minute % 10 + '0'); //print minute nd.
      print_tiny_char(25, 2, second / 10 + '0'); //print second st.
      print_tiny_char(29, 2, second % 10 + '0'); //print second nd.
}
void Set_Time_Minute(){
  if(minute < 0) {minute = 59;}
  else if(minute > 59) {minute = 0;}
  if(state_Plus == 1) {minute++;}
  if(state_Minus == 1) {minute--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
      print_tiny_char(13, 2, minute / 10 + '0'); //print minute st.
      print_tiny_char(17, 2, minute % 10 + '0'); //print minute nd.
  }
  if (now - lastblink >= 600){
      Clear_Second();
      lastblink = now;
  }
      Draw_Blink();
  //Print the rest of it
      print_tiny_char( 1, 2,  hour / 10 + '0'); //print hour st.
      print_tiny_char( 5, 2,  hour % 10 + '0'); //print hour nd.
      print_tiny_char(25, 2, second / 10 + '0'); //print second st.
      print_tiny_char(29, 2, second % 10 + '0'); //print second nd.
}
void Set_Time_Second(){
  if(second < 0) {second = 59;}
  else if(second > 59) {second = 0;}
  if(state_Plus == 1) {second++;}
  if(state_Minus == 1) {second--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
      print_tiny_char(25, 2, second / 10 + '0'); //print second st.
      print_tiny_char(29, 2, second % 10 + '0'); //print second nd.
  }
  if (now - lastblink >= 600){
      Clear_Third();
      lastblink = now;
  }
      Draw_Blink();
  //Print the rest of it
      print_tiny_char( 1, 2,  hour / 10 + '0'); //print hour st.
      print_tiny_char( 5, 2,  hour % 10 + '0'); //print hour nd.
      print_tiny_char(13, 2, minute / 10 + '0'); //print minute st.
      print_tiny_char(17, 2, minute % 10 + '0'); //print minute nd.
}
/////////////////////////////////////////////////// Set Day/Month/Year /////////////////////////////////////////////
void Set_Day(){
  if(day< 1) {day = day_month[month-1];}
  else if(day > day_month[month-1]) {day = 1;}
  if(state_Plus == 1) {day++;}
  if(state_Minus == 1) {day--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
        print_tiny_char( 1, 2,  day / 10 + '0'); //print day st.
        print_tiny_char( 5, 2,  day % 10 + '0'); //print day nd.
  }
  if (now - lastblink >= 600){
      Clear_First();
      lastblink = now;
  }
        Draw_Slash();
  //Print Day...
        print_tiny_char(13, 2, month / 10 + '0'); //print month st.
        print_tiny_char(17, 2, month % 10 + '0'); //print month nd.
        print_tiny_char(25, 2, year / 10 + '0'); //print year st.
        print_tiny_char(29, 2, year % 10 + '0'); //print year nd.
}
void Set_Month(){
  if(month< 0) {month = 12;}
  else if(month > 12) {month = 0;}
  if(state_Plus == 1) {month++;}
  if(state_Minus == 1) {month--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
        print_tiny_char(13, 2, month / 10 + '0'); //print month st.
        print_tiny_char(17, 2, month % 10 + '0'); //print month nd.
  }
  if (now - lastblink >= 600){
      Clear_Second();
      lastblink = now;
  }
        Draw_Slash();
  //Print Day...
        print_tiny_char( 1, 2,  day / 10 + '0'); //print day st.
        print_tiny_char( 5, 2,  day % 10 + '0'); //print day nd.
        print_tiny_char(25, 2, year / 10 + '0'); //print year st.
        print_tiny_char(29, 2, year % 10 + '0'); //print year nd.
}
void Set_Year(){
  if(year< 0) {year = 99;}
  else if(year > 99) {year = 0;}
  if(state_Plus == 1) {year++;}
  if(state_Minus == 1) {year--;}

  //Make it blink to Set new value
  if(now - lastblink >= 300){
        print_tiny_char(13, 2, month / 10 + '0'); //print month st.
        print_tiny_char(17, 2, month % 10 + '0'); //print month nd.
  }
  if (now - lastblink >= 600){
      Clear_Third();
      lastblink = now;
  }
        Draw_Slash();
  //Print Day...
        print_tiny_char( 1, 2,  day / 10 + '0'); //print day st.
        print_tiny_char( 5, 2,  day % 10 + '0'); //print day nd.
        print_tiny_char(25, 2, year / 10 + '0'); //print year st.
        print_tiny_char(29, 2, year % 10 + '0'); //print year nd.
}
///////////////////////////////////////////////////////// Draw Words //////////////////////////////////////////
void Draw_Alarm(){
      print_tiny_char( 6, 2, 'A');
      print_tiny_char( 11, 2, 'L');
      print_tiny_char( 16, 2, 'A');
      print_tiny_char( 21, 2, 'R');
      print_tiny_char( 26, 2, 'M');
}
void Draw_OnOff(){
      print_tiny_char( 3, 2, 'O');
      print_tiny_char( 7, 2, 'N');
      print_tiny_char( 15, 2, 'O');
      print_tiny_char( 19, 2, 'F');
      print_tiny_char( 23, 2, 'F');
}
void Draw_Wakeup(){
      print_tiny_char( 3, 2, 'W');
      print_tiny_char( 7, 2, 'A');
      print_tiny_char( 11, 2, 'K');
      print_tiny_char( 15, 2, 'E');
      print_tiny_char( 23, 2, 'U');
      print_tiny_char( 27, 2, 'P');
}
void Draw_StopWatch(){
      print_tiny_char( 1, 2, 'S');
      print_tiny_char( 5, 2, 'T');
      print_tiny_char( 9, 2, 'P');
      print_tiny_char( 14, 2, 'W');
      print_tiny_char( 18, 2, 'A');
      print_tiny_char( 22, 2, 'T');
      print_tiny_char( 26, 2, 'C');
      print_tiny_char( 30, 2, 'H');
      plot(11,6,1);
}
void Draw_Timer(){
      print_tiny_char( 6, 2, 'T');
      print_tiny_char( 11, 2, 'I');
      print_tiny_char( 16, 2, 'M');
      print_tiny_char( 21, 2, 'E');
      print_tiny_char( 26, 2, 'R');
}
void Draw_SetTime(){
      print_tiny_char( 3, 2, 'S');
      print_tiny_char( 7, 2, 'E');
      print_tiny_char( 11, 2, 'T');
      print_tiny_char( 16, 2, 'T');
      print_tiny_char( 20, 2, 'I');
      print_tiny_char( 24, 2, 'M');
      print_tiny_char( 28, 2, 'E');
}
void Draw_SetDMY(){
      print_tiny_char( 3, 2, 'S');
      print_tiny_char( 7, 2, 'E');
      print_tiny_char( 11, 2, 'T');
      print_tiny_char( 17, 2, 'D');
      print_tiny_char( 21, 2, 'M');
      print_tiny_char( 25, 2, 'Y');
}
///////////////////////////////////////////////////////// Matrix Functions //////////////////////////////////////////
void Draw_Blink(){
    if (blinking){
      plot(10, 3, 0);
      plot(10, 5, 0);
      plot(22, 3, 0);
      plot(22, 5, 0);
    }
    else{
      plot(10, 3, 1);
      plot(10, 5, 1);
      plot(22, 3, 1);
      plot(22, 5, 1);
    }
}
void Draw_Slash(){
      plot(9,5,1);
      plot(10,4,1);
      plot(11,3,1);
      plot(21,5,1);
      plot(22,4,1);
      plot(23,3,1);
}
void Change_Intensity(){
  //Auto Bright *** 
  intensity = map(Get_buffer(), 800, 300 , 0, 9);
  //intensity = 0;
  for (int address = 0; address < 4; address++){
      lc.setIntensity(address, intensity);
  }
}
void Change_Screen(){
  for (int y = 1; y <= 8; y++){
    for (int x = 1; x <= 32; x++){  plot(x,y,1); }
      //delay(5);
  }
  for (int y = 1; y <= 8; y++){
    for (int x = 1; x <= 32; x++){  plot(x,y,0); }
      //delay(5);
  }
}
void Clear_Display(){
  for (int y = 1; y <= 8; y++){
    for (int x = 1; x <= 32; x++){  plot(x,y,0); }
  }
}
void Clear_First(){
  for (int y = 2; y <= 6; y++){
    for (int x = 1; x <= 7; x++){  plot(x,y,0); }
  }
}
void Clear_Second(){
  for (int y = 2; y <= 6; y++){
    for (int x = 13; x <= 19; x++){  plot(x,y,0); }
  }
}
void Clear_Third(){
  for (int y = 2; y <= 6; y++){
    for (int x = 25; x <= 31; x++){  plot(x,y,0); }
  }
}
void plot (int x, int y, bool value) {
  x = x-1;
  y = y-1;
  int address;                                      
  if (x >= 0  && x <= 7)  { address = 3; }
  if (x >= 8  && x <= 15) { address = 2; x = x - 8; }
  if (x >= 16 && x <= 23) { address = 1; x = x - 16; }
  if (x >= 24 && x <= 31) { address = 0; x = x - 24; }
  lc.setLed(address, y, x, value);                      //Transpose Already
}

void print_tiny_char(int x, int y, char c) {
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') )  c &= 0x1F;          
  else if (c >= '0' && c <= '9')                        c  = (c - '0') + 32;
  else if (c == ' ' )                                   c  =  0;        
  else if (c == '.' )                                   c  = 27;           
  else if (c == ':' )                                   c  = 28;          
  else if (c == '\'')                                   c  = 29;             
  else if (c == '!' )                                   c  = 30;             
  else if (c == '?' )                                   c  = 31;               

  for (uint8_t col = 0; col < 3; col++)
  {
    dots = pgm_read_byte_near(&mytinyfont[c][col]);
    for (uint8_t row = 0; row < 5; row++)
    {
      if (dots & (16 >> row)) 
        plot(x + col, y + row, 1);
      else 
        plot(x + col, y + row, 0);
    }
  }
}

///////////////////////////////////////////////////////// Buffer Functions //////////////////////////////////////////
int Get_buffer() {

  buffer_Value[buffer_Position] = analogRead(LDR_PIN);
  buffer_Position = (buffer_Position + 1) % SIZE;

  long sum = 0;
  for (unsigned int i = 0; i < SIZE; i++)
  sum += buffer_Value[i];

  return round(sum / SIZE);
}
