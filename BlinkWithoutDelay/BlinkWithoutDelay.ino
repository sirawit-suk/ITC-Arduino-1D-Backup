
const int R =  4;
const int Y =  3;
const int G =  2;// the number of the LED pin

int RED = LOW;
int YELLOW = LOW; 
int GREEN = LOW;   // ledState used to set the LED

unsigned long past1 = 0;
unsigned long p2 = 0;
unsigned long p3 = 0;  // will store last time LED was updated


const long interval1 = 200;
const long interval2 = 400; 
const long interval3 = 600; // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(Y, OUTPUT);
  pinMode(G, OUTPUT);
}

void loop() {
  
  unsigned long current = millis();

  if (current - past1 >= interval1) {
    
    past1 = current;

    if (RED == LOW) {
      RED = HIGH;
    } else if(RED == HIGH){
      RED = LOW;
    }

    digitalWrite(R, RED);
  }
/*
  if (c - p2 >= interval2) {
    
    p2 = c;

    if (YELLOW == LOW) {
      YELLOW = HIGH;
    } else {
      YELLOW = LOW;
    }

    digitalWrite(Y, YELLOW);
  }

  if (c - p3 >= interval3) {
    
    p3 = c;

    if (GREEN == LOW) {
      GREEN = HIGH;
    } else {
      GREEN = LOW;
    }

    digitalWrite(G, GREEN);
  }

  */
}
