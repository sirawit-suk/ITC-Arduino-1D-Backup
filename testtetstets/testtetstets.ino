
void setup() {
 Serial.begin(9600);
}
void loop()
{
 //Sine Wave & Cosine Wave
 float angle=0;
 for(angle=0;angle<=90;angle=angle+0.1)
 {
 float sina=sin(angle);
 float cosa=cos(angle);
 
 Serial.print(sina);
 Serial.print(" ");
 Serial.print(cosa);
 Serial.print(" ");
 Serial.println(0.5);
 delay(1);
 }
}
