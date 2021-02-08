#define button1 12
#define button2 11
#define button3 10
#define R 4
#define Y 3
#define G 2

unsigned long past1 = 0;
unsigned long past2 = 0;
unsigned long past3 = 0;

unsigned long last1 = 0;
unsigned long last2 = 0;
unsigned long last3 = 0;

int LastButton1=0;
int LastButton2=0;
int LastButton3=0;

int S1;
int S2;
int S3;

int Button1=0;
int Button2=0;
int Button3=0;

int RED = LOW;
int YELLOW = LOW; 
int GREEN = LOW;

const long interval1 = 50; //R
const long interval2 = 3000; //Y
const long interval3 = 3000; //G

void LightOnOff(int,int,int);
void RedLight(int);
void YellowLight(int);
void GreenLight(int);

int Input1(int);
int Input2(int);
int Input3(int);

unsigned long now;

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
  S1 = digitalRead(button1);
  S2 = digitalRead(button2);
  S3 = digitalRead(button3);
  now = millis();
  
  LightOnOff(Input1(S1),Input2(S2),Input3(S3));

} 

int Input1(int x){
  if(x==1 && now - last1 >= 200){
    Serial.println("Press 1");
    last1 = now;
  }
  return x;
}
int Input2(int x){
  if(x==0 && now - last2 >= 200){
    Serial.println("Press 2");
    last2 = now;
  }
  return x;
}
int Input3(int x){
  if(x==0 && now - last3 >= 200){
    Serial.println("Press 3");
    last3 = now;
  }
  return x;
}


void LightOnOff(int red,int yellow,int green){
    RedLight(red);
    YellowLight(yellow);
    GreenLight(green);
}

void RedLight(int x){
   
   if(x != LastButton1){
      past1=now;
   }
   if(now - past1>=interval1){

        if(x != Button1){
          Button1 = x;
          
              if(Button1 == HIGH){
                RED = !RED;
              }
        }
   }
   digitalWrite(R, RED); 
   LastButton1=x;

}

void YellowLight(int x){
  
   if(x == LOW){
    
      YELLOW = HIGH;
      
      past2=now;
   }
   if(now - past2>=interval2){
      YELLOW = LOW;
   }
   digitalWrite(Y, YELLOW);
  
}

void GreenLight(int x){
   
   if(x == LOW){
      GREEN = HIGH;
      past3=now;
   }
   if(now - past3>=interval3){
      GREEN = LOW;
   }
   digitalWrite(G, GREEN);
}
