#define button1 12
#define button2 11
#define button3 10
#define R 4
#define Y 3
#define G 2

unsigned long past1 = 0;
unsigned long past2 = 0;
unsigned long past3 = 0;

int RED = LOW;
int YELLOW = LOW; 
int GREEN = LOW;

const long interval1 = 3000; //R
const long interval2 = 500;  //Y
const long interval3 = 3000; //G

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
  int S1 = digitalRead(button1);
  int S2 = digitalRead(button2);
  int S3 = digitalRead(button3);


      
  if(S1 == HIGH){                         //PULLDOWN
    Serial.println("Pressed S1");
    digitalWrite(R,HIGH);
    delay(500);
  }
  else{
    digitalWrite(R,LOW);
  }

                                          //PULLUP 
  if(S2 == LOW){
    Serial.println("Pressed S2");
    digitalWrite(Y,HIGH);
    delay(500);
  }
  else{
    digitalWrite(Y,LOW);
  }

                                          //INTERNAL_PULLUP
  if(S3 == LOW){
    Serial.println("Pressed S3");
    digitalWrite(G,HIGH);
    delay(500);
  }
  else{
    digitalWrite(G,LOW);
  }




}
