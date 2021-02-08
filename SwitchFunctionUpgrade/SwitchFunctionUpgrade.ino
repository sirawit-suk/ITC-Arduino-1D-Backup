#define button1 12 
#define button2 11 
#define button3 10
#define R 4 
#define Y 3 
#define G 2
unsigned long now;
unsigned long past1 = 0,past2 = 0,past3 = 0;
unsigned long last1 = 0,last2 = 0,last3 = 0;

bool S1,S2,S3;

bool state1,state3,Ystate=0;
int state2,count;

bool RED = LOW,YELLOW = LOW,GREEN = LOW;

void LightOnOff(bool,bool,bool);
bool RedLight();
bool YellowLight();
bool GreenLight();

void CheckInput();
void Input1(bool); 
void Input2(bool); 
void Input3(bool);

void setup() {
  Serial.begin(9600);
  pinMode(button1,INPUT);//DOWN
  pinMode(button2,INPUT);//UP
  pinMode(button3,INPUT_PULLUP);//INTERNAL
  pinMode(R,OUTPUT);
  pinMode(Y,OUTPUT);
  pinMode(G,OUTPUT);
}

void loop() {
  now = millis();

  //CheckInput();
  LightOnOff(RedLight(),YellowLight(),GreenLight());

}

void LightOnOff(bool RED,bool YELLOW,bool GREEN){
    digitalWrite(R,RED);
    digitalWrite(Y,YELLOW);
    digitalWrite(G,GREEN);
}

bool RedLight(){
  S1 = digitalRead(button1);
  if(S1 == HIGH && now-past1>=200){
    past1=now;
    RED = !RED;
  }
  else if(now-past1>=3000){
    RED = LOW;
  }
   return RED;
}

bool YellowLight(){
  S2 = digitalRead(button2);
  if(S2 == LOW && now-past2>=200 && !RED && !GREEN){
      if(Ystate == 0){
        YELLOW = HIGH;//on
        Ystate = 1;
        
      }else{
        YELLOW = LOW;//off
        Ystate = 0;
      }
      past2=now;
  }
  else if(now-past2>=500 && now-past2<1000 && Ystate == 1){
    YELLOW = LOW; // off
    Ystate = 1;
  }
  else if(now-past2>=1000 && now-past2<1500 && Ystate == 1){
    YELLOW = HIGH; //on
    Ystate = 1;
  }
  else if(now-past2>=1500 && now-past2<2000 && Ystate == 1){
    YELLOW = LOW; //off
    Ystate = 0;
  }
   return YELLOW;
}

bool GreenLight(){
  S3 = digitalRead(button3);
  if(S3 == LOW && now-past3>=200 && !RED){
    past3=now;
    GREEN = !GREEN;
  }  
  else if(now-past3>=3000){
    GREEN = LOW;
  }
   return GREEN;
}
 
/*
void CheckInput(){
  Input1(S1 = digitalRead(button1));
  Input2(S2 = digitalRead(button2));
  Input3(S3 = digitalRead(button3));
}

void Input1(bool x){
  if(x == 1 && now - last1>= 200){
    if(state1 == 0){
      Serial.println("RED : Turn On");
      state1 = 1;
    }else{
      Serial.println("RED : Turn Off");  
      state1 = 0;
    }
    last1 = now;
    
  }
  else if(now - last1>= 3000 && state1 == 1){
      Serial.println("RED : Turn Off");
      state1 = 0;
  }
}
void Input2(bool x){
  if(x == 0 && now - last2>= 200){
    if(state2 == 0){
      Serial.println("YELLOW : Turn On");
      state2 = 1;
    }else{
      if(count==1){
        count=0; 
      }
      else{
        Serial.println("YELLOW : Turn Off");
        count=0;
      }
      state2 = 0;    
    }
    last2 = now;
  }
  else if(now - last2>= 500 && now - last2 < 1000 && state2 == 1){
      Serial.println("YELLOW : Turn Off");
      count = 1;
      state2 = 2;
  }
  else if(now - last2>= 1000 && now - last2 < 1500 && state2 == 2){
      Serial.println("YELLOW : Turn On");
      count = 0;
      state2 = 3;
  }
  else if(now - last2>= 1500 && state2 == 3){
      Serial.println("YELLOW : Turn Off");
      count = 1;
      state2 = 0;
  }
}
void Input3(bool x){
  if(x == 0 && now - last3>= 200 && !RED){
    if(state3 == 0){
      Serial.println("GREEN : Turn On");
      state3 = 1;
    }else{
      Serial.println("GREEN : Turn Off");  
      state3 = 0;
    }
    last3 = now;
    
  }
  else if(now - last3>= 3000 && state3 == 1){
      Serial.println("GREEN : Turn Off");
      state3 = 0;
  }
}
*/
