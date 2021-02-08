bool x = 1;
double TIME = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
}

void loop() {
/*
  float angle = 0;
  for(angle=0; angle<=90; angle = angle + 0.1){
    float SIN_A = sin(angle);
    float COS_A = cos(angle);
    Serial.print(SIN_A);
    Serial.print(" ");
    Serial.println(COS_A);
  }
    delay(1);
*/
/*
  for(TIME = 0; TIME <100; TIME+=1){  
    Serial.println(x);
    delay(10);
  }
  x = !x;
*/
  float analog = analogRead(A0);
  float digital = digitalRead(A1); 
  Serial.print(analog);
  Serial.print(" ");
  Serial.println(digital);
 
}
