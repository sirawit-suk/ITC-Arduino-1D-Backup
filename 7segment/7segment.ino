#define a 2
#define b 3
#define c 4
#define dp 5
#define d 6
#define e 7
#define g 8
#define f 9
#define s1 10
#define s2 12
#define sd 11
#define green A2
#define red A1

bool A=1,B=1,C=1,D=1,E=1,F=1,G=1,DP=1; // 0 is On , 1 is Off // Negative Logic 
unsigned long int now,last1=0,last2=0;
int state1=0,randNum = 0,baseNum = 0;

void setup() {
  Serial.begin(9600);
  pinMode(a,OUTPUT);pinMode(b,OUTPUT);pinMode(c,OUTPUT);pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);pinMode(f,OUTPUT);pinMode(g,OUTPUT);pinMode(dp,OUTPUT);
  pinMode(s1,INPUT_PULLUP);
  pinMode(s2,INPUT_PULLUP);
  pinMode(sd,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  analogWrite(green,255); //255 is turn off, 0 is turn on
  analogWrite(red,255);
  LightOut(1,1,1,1,1,1,1,1);
  randomSeed(analogRead(0));
}

void loop() {
  CheckPressA();
  CheckPressB();
}
void LightOut(bool A, bool B, bool C, bool D, bool E, bool F, bool G, bool DP){ 
  digitalWrite(a,A); digitalWrite(b,B); digitalWrite(c,C); digitalWrite(d,D);
  digitalWrite(e,E); digitalWrite(f,F); digitalWrite(g,G); digitalWrite(dp,DP);
}
void CheckPressA (){
  now = millis();
  bool S1 = digitalRead(s1);
  if(S1 == 0 && now - last1 >= 200){
    state1++;
    last1 = now;
    Beep(100);
  }
  else if(state1 == 1){LightOut(1,0,0,1,1,1,1,1); baseNum = 1;}
  else if(state1 == 2){LightOut(0,0,1,0,0,1,0,1); baseNum = 2;}
  else if(state1 == 3){LightOut(0,0,0,0,1,1,0,1); baseNum = 3;}
  else if(state1 == 4){LightOut(1,0,0,1,1,0,0,1); baseNum = 4;}
  else if(state1 == 5){LightOut(0,1,0,0,1,0,0,1); baseNum = 5;}
  else if(state1 == 6){LightOut(0,1,0,0,0,0,0,1); baseNum = 6;state1 = 0;} 
}
void CheckPressB (){
  now = millis();
  bool S2 = digitalRead(s2);
  if(S2 == 0 && now - last2 >= 200){
    randNum = random(1,7);
    Serial.print("Your Random Number is : ");
    Serial.print(randNum);
    if (baseNum == randNum) Pass(); else Fail();
    last2 = now;
    Beep(100);
  }
}
void Pass(){
  Serial.println(" You're Pass! Congraturation");
  analogWrite(red,255);  //255 is turn off, 0 is turn on
  analogWrite(green,0);
  for(int i = 0; i<5000; i++ ){LightOut(0,0,1,1,0,0,0,0);} 
}
void Fail(){
  Serial.println(" Fail! Try again!");
  analogWrite(green,255);
  analogWrite(red,0);
  for(int i = 0; i<5000; i++ ){LightOut(0,1,1,1,0,0,0,0);}
}
void Beep(int duration){
  analogWrite(sd,1000);
  delay(duration);
  analogWrite(sd,0);
  delay(duration);
}
