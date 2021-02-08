// motor one
#define lt_L2 2
#define lt_L1 3
#define lt_M 4
#define lt_R1 12
#define lt_R2 13
#define button 11

#define ss_L2 A0
#define ss_L1 A1
#define ss_M A2
#define ss_R1 A3
#define ss_R2 A4

#define enA 5
#define in1 6
#define in2 7

#define enB 10  
#define in3 9    //Swap port in Program because hardware motor are swap
#define in4 8

//const unsigned int NUM_AXES = 5;
//const unsigned int PINS[NUM_AXES] = { ss_L2, ss_L1, ss_M, ss_R1, ss_R2 };
//const unsigned int BUFFER_SIZE = 16;
//int buffer[NUM_AXES][BUFFER_SIZE];
//int buffer_pos[NUM_AXES] = { 0 };


 
float sensor_L2 = 0.f, sensor_L1 = 0.f, sensor_M = 0.f, sensor_R1 = 0.f, sensor_R2 = 0.f;
bool light_L2 = LOW, light_L1 = LOW, light_M = LOW, light_R1 = LOW, light_R2 = LOW;

bool isCalibrate_dark = false;
bool isCalibrate_light = false;

float interval_start[5] = {};
float interval_end[5] = {};
float interval[5] = {};

int dt = 500;

int speed_L = 250;
int speed_R = 250;

bool buttonCali = true;

void setup(){

  Serial.begin(9600);
  
  // set arduino pins output to motor pins 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(lt_L2, OUTPUT);
  pinMode(lt_L1, OUTPUT);
  pinMode(lt_M, OUTPUT);
  pinMode(lt_R1, OUTPUT);
  pinMode(lt_R2, OUTPUT);

  pinMode(ss_L2, INPUT);
  pinMode(ss_L1, INPUT);
  pinMode(ss_M, INPUT);
  pinMode(ss_R1, INPUT);
  pinMode(ss_R2, INPUT);

  pinMode(button, INPUT_PULLUP);
 
}
void loop()
{

  //SensorInput();
  //if(isCalibrate_dark == true && isCalibrate_light){
    //LightOutput();  //For Debugging and Checkstate
    //CheckState();
  //}

  //delay(200);
    AllDelay();
    PrintTest();
    
}
void AllDelay()
{
//GOOOOOOOOOOOOO
  Forward(speed_L,speed_R);
  delay(560);
  Stop(500);
  
//TURN RIGHT AND GOOOOOOOO AND TURN LEFT AND GOOOOOO
  TurnRight(speed_L-50,0);
  delay(200);
  Stop(560);
  
  Forward(speed_L,speed_R);
  delay(400);
  Stop(560);

  TurnLeft(0,speed_R-50);
  delay(372);
  Stop(560);

  Forward(speed_L,speed_R);
  delay(800);
  Stop(560);
  
//TURN RIGHT AND GOOOOOOOO AND TURN LEFT AND GOOOOOO
  TurnRight(speed_L-50,0);
  delay(360);
  Stop(560);

  Forward(speed_L,speed_R);
  delay(700);
  Stop(500);

  TurnLeft(0,speed_R-50);
  delay(385);
  Stop(560);

  Forward(speed_L,speed_R);
  delay(500);
  Stop(560);

///////////////////////////////////
//BEFORE GO BIG STATE
/////////////////////////////////
  TurnRight(speed_L-50,0);
  delay(450);
  Stop(560);

  Forward(speed_L,speed_R);
  delay(300);
  Stop(500);

  
  delay(10000);
}
////////////////////////////////////////////////////// Check State ///////////////////////////////////////
void CheckState()
{
  if(light_L2 == LOW && light_R2 == HIGH)
  {
    TurnRight(speed_L+40, 0);
    // Serial.print("FUCKKKKKKKKKKKKKK1");
  }
  else if(light_L2 == HIGH && light_R2 == LOW)
  {
    TurnLeft(0, speed_R+40);
    //Serial.print("FUCKKKKKKKKKKKKKK2");
  }
  else if(light_L1 == LOW && light_R1 == HIGH)
  {
    TurnRight(speed_L+40, speed_R-40);
   // Serial.print("FUCKKKKKKKKKKKKKK5");
  }
  else if(light_L1 == HIGH && light_R1 == LOW)
  {
    TurnLeft(speed_L-40, speed_R+40);
   // Serial.print("FUCKKKKKKKKKKKKKK6");
  }
  else if (light_M == HIGH)
  {
    Forward(speed_L,speed_R);
  }
  else
  {
    TurnRight(speed_L+40, 0);
       // Serial.print("FUCKKKKKKKKKKKKKK7");
  }
    
}
////////////////////////////////////////////////////// Sensor INPUT ///////////////////////////////////////
void SensorInput()
{
  sensor_L2 = analogRead(ss_L2);
  sensor_L1 = analogRead(ss_L1);
  sensor_M = analogRead(ss_M);
  sensor_R1 = analogRead(ss_R1);
  sensor_R2 = analogRead(ss_R2);
  
  buttonCali = digitalRead(button);
  /*
  if(buttonCali == 0)
  {
    if(isCalibrate_dark == false){
      isCalibrate_dark = true;
      for(int i = 0; i<5 ; i++)
      {
        interval_start[i] = sensor_Value(i);      //Calibrate in the DARK ONLY!!!!! 
      }
       delay(2000);
    }
    else if(isCalibrate_light == false){
      isCalibrate_light = true;
      for(int i = 0; i<5 ; i++)
      {
        interval_end[i] = sensor_Value(i);      //Calibrate in the DARK ONLY!!!!! 
      }

      Calibrate();
      delay(2000);
    }
  }
  */
  
}
void PrintTest()
{
  /*
}
  Serial.print("L2: ");
  Serial.print(sensor_Value(0));
  Serial.print("  L1: ");
  Serial.print(sensor_Value(1));
  Serial.print("  M: ");
  Serial.print(sensor_Value(2));
  Serial.print("  R1: ");
  Serial.print(sensor_Value(3));
  Serial.print("  R2: ");
  Serial.println(sensor_Value(4));
*//*
  Serial.print("L2: ");
  Serial.print(sensor_L2);
  Serial.print("  L1: ");
  Serial.print(sensor_L1);
  Serial.print("  M: ");
  Serial.print(sensor_M);
  Serial.print("  R1: ");
  Serial.print(sensor_R1);
  Serial.print("  R2: ");
  Serial.println(sensor_R2);
  */
    Serial.print("L2: ");
  Serial.print(light_L2);
  Serial.print("  L1: ");
  Serial.print(light_L1);
  Serial.print("  M: ");
  Serial.print(light_M);
  Serial.print("  R1: ");
  Serial.print(light_R1);
  Serial.print("  R2: ");
  Serial.println(light_R2);
  
}
void Calibrate(){

  for(int i = 0; i< 5 ; i++)
  {
    interval[i] = (interval_start[i] + interval_end[i])/2.f;  
  }
  
}

////////////////////////////////////////////////////// Light Debugging Output ///////////////////////////////////////
void LightOutput()
{
//****** L2 ******//*                //LIGHT                    //DARK
  if(sensor_L2 > (500)) { light_L2 = HIGH; } else { light_L2 = LOW; }
  
//****** L1 ******//*
  if(sensor_L1 > (300+450)/2.f) { light_L1 = HIGH; } else { light_L1 = LOW; }

//****** M ******//*
  if(sensor_M > (390)) { light_M = HIGH; } else { light_M = LOW; }

//****** R1 ******//*
  if(sensor_R1 > (300+500)/2.f) { light_R1 = HIGH; } else { light_R1 = LOW; }

//****** R2 ******//*
  if(sensor_R2 > (400)) { light_R2 = HIGH; } else { light_R2 = LOW; }
  
  digitalWrite(lt_L2,light_L2);
  digitalWrite(lt_L1,light_L1);
  digitalWrite(lt_M,light_M);
  digitalWrite(lt_R1,light_R1);
  digitalWrite(lt_R2,light_R2);
}

////////////////////////////////////////////////////// Movement Functions ///////////////////////////////////////
//***** FORWARD / BACKWARD ****
void Forward(int L , int R){
  go_L();
  go_R();
  analogWrite(enA, L);
  analogWrite(enB, R);
}
void Backward(int L , int R){
  back_L();
  back_R();
  analogWrite(enA, L);
  analogWrite(enB, R);
}

//***** STOP ****
void Stop(int x){
  stop_L();
  stop_R();
  delay(x);
}

//***** TURN RIGHT ****

void TurnRight(int L , int R){
  go_L();
  go_R();
  analogWrite(enA, L);
  analogWrite(enB, R);
}

//***** TURN LEFT ****
void TurnLeft(int L , int R){
  go_R();
  go_L();
  analogWrite(enA, L);
  analogWrite(enB, R);
}

///////////////////////////////////////////////////// Wheel Elements ///////////////////////////////////////////
void go_L(){
  digitalWrite(in1, HIGH);      //Don't make it High both
  digitalWrite(in2, LOW);
}
void go_R(){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void back_L(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
void back_R(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void stop_L(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
void stop_R(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

///////////////////////////////////////////////////// Jitter Function ///////////////////////////////////////////
/*
int sensor_Value(const int axis) {
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  long sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
  sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}
*/
