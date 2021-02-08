const int button1 = 12;   
const int R = 4;     

int state = HIGH;        
int BTstate;             
int LASTBTstate = LOW;   


unsigned long LAST = 0;  // the last time the output pin was toggled
unsigned long DEBOUNCE = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(button1, INPUT);
  pinMode(R, OUTPUT);
  digitalWrite(R, HIGH);
}

void loop() {

  int READ = digitalRead(button1);

  if (READ != LASTBTstate) {

    LAST = millis();
  }

  if ((millis() - LAST) > DEBOUNCE) {

    if (READ != BTstate) {
      BTstate = READ;

      if (BTstate == HIGH) {
        state = !state;
      }
    }
  }

  digitalWrite(R, state);
  LASTBTstate = READ;
}
