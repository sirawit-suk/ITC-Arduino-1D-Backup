///////////////////////////////////// BUTTON PINS //////////////////////////////////////////////
#define NORTH_BUTTON_PIN 2
#define EAST_BUTTON_PIN 3
#define WALK_BUTTON_PIN 4

///////////////////////////////////// LED TRAFIC (NORTH+EAST) /////////////////////////////////
#define LED_N_R 5
#define LED_N_Y 6
#define LED_N_G 7
#define LED_E_R 8
#define LED_E_Y 9
#define LED_E_G 10
#define LED_WALK_G 11
#define LED_WALK_R 12

///////////////////////////////////// Number of stages /////////////////////////////////////////
#define goNorth 0
#define waitNorth 1
#define waitN_ToW 2
#define goEast 3
#define waitEast 4
#define waitE_ToW 5
#define goWalk 6
#define StopToN1 7
#define ToN1 8
#define StopToN2 9
#define ToN2 10
#define StopToE1 11
#define ToE1 12
#define StopToE2 13
#define ToE2 14

///////////////////////////////////// Create state w/ struct //////////////////////////////////
struct SType {
  unsigned long ST_Out;   // OUTPUT Pattern
  unsigned long Time;     // Delay
  unsigned long Next[8];  // Next State
};
//State ; //typedef const struct State SType;

//////////////////////////////////// Name of Next Stages ///////////////////////////////////////////
SType FSM[15] = {
  {B00110001, 1000, {goNorth, goNorth, waitNorth, waitNorth, waitN_ToW, waitN_ToW, waitNorth, waitNorth}},     //goNorth    1
  {B01010001, 500,  {goEast,  goEast, goEast, goEast, goWalk, goWalk, goWalk, goEast}},                         //waitNorth  2
  {B01010001, 500,  {goWalk,  goWalk, goWalk, goWalk, goWalk, goWalk, goWalk, goWalk}},                         //waitN_ToW  3
  
  {B10000101, 1000, {goEast,  waitEast, goEast, waitEast, waitE_ToW, waitEast, waitE_ToW, waitEast}},          //goEast     4
  {B10001001, 500,  {goNorth, goNorth, goNorth, goNorth, goWalk, goWalk, goWalk, goWalk}},                      //waitEast   5
  {B10001001, 500,  {goWalk,  goWalk,  goWalk,  goWalk, goWalk, goWalk, goWalk, goWalk}},                       //waitE_ToW  6
  
  {B10010010, 1000, {goWalk,  StopToN1, StopToE1, StopToN1, goWalk, StopToN1, StopToE1, StopToN1}},            //goWalk     7
  
  {B10010000, 500,  {ToN1, ToN1, ToN1, ToN1, ToN1, ToN1, ToN1, ToN1}},                                             //StopToN1   8
  {B10010010, 500,  {StopToN2, StopToN2, StopToN2, StopToN2, StopToN2, StopToN2, StopToN2, StopToN2}},             //ToN1       9
  {B10010000, 500,  {ToN2, ToN2, ToN2, ToN2, ToN2, ToN2, ToN2, ToN2}},                                             //StopToN2   10
  {B10010010, 500,  {goNorth, goNorth, goEast, goNorth, goWalk, goNorth, goEast, goNorth}},                        //ToN2       11
  
  {B10010000, 500,  {ToE1, ToE1, ToE1, ToE1, ToE1, ToE1, ToE1, ToE1}},                                             //StopToE1   12
  {B10010010, 500,  {StopToE2, StopToE2, StopToE2, StopToE2, StopToE2, StopToE2, StopToE2, StopToE2}},             //ToE1       13
  {B10010000, 500,  {ToE2, ToE2, ToE2, ToE2, ToE2, ToE2, ToE2, ToE2}},                                             //StopToE2   14
  {B10010010, 500,  {goEast, goNorth, goEast, goNorth, goWalk, goNorth, goEast, goNorth}}                          //ToE2       15
};  

unsigned long S = 0; // Stage starts form zero

/////////////////////////////////// LED and Buttons Setup /////////////////////////////////////////
void setup() {
  pinMode(LED_N_R, OUTPUT);
  pinMode(LED_N_Y, OUTPUT);
  pinMode(LED_N_G, OUTPUT);
  pinMode(NORTH_BUTTON_PIN, INPUT);
  pinMode(LED_E_R, OUTPUT);
  pinMode(LED_E_Y, OUTPUT);
  pinMode(LED_E_G, OUTPUT);
  pinMode(EAST_BUTTON_PIN, INPUT);
  pinMode(LED_WALK_G, OUTPUT);
  pinMode(LED_WALK_R, OUTPUT);
  pinMode(WALK_BUTTON_PIN, INPUT);
  Serial.begin(9600);
}

///////////////////////////////// uses logic with LED state //////////////////////////////////////////
int input, goNorth1, goEast2, goWalk3;
void loop() {

  //NORTH LED (GO NORTH)                  /(N)/R Y G /(E)/ R Y G /(W)/ R G
  digitalWrite(LED_N_R, !(FSM[S].ST_Out & B10000000));
  digitalWrite(LED_N_Y, !(FSM[S].ST_Out & B01000000));
  digitalWrite(LED_N_G, !(FSM[S].ST_Out & B00100000));
  
  //EAST LED (GO EAST)
  digitalWrite(LED_E_R, !(FSM[S].ST_Out & B00010000));
  digitalWrite(LED_E_Y, !(FSM[S].ST_Out & B00001000));
  digitalWrite(LED_E_G, !(FSM[S].ST_Out & B00000100));
  
  //CROSS WALK LED (GO WALK)
  digitalWrite(LED_WALK_G, !(FSM[S].ST_Out & B00000010));
  digitalWrite(LED_WALK_R, !(FSM[S].ST_Out & B00000001));
  //DELAY STATE
  delay(FSM[S].Time);

  //ALL INPUT
  goNorth1 = digitalRead(NORTH_BUTTON_PIN);
  goEast2 = digitalRead(EAST_BUTTON_PIN);
  goWalk3 = digitalRead(WALK_BUTTON_PIN);
 
  //87,654,321
  input = goNorth1  + (goEast2  * 2)  + (goWalk3 * 4);
  Serial.println(input);

  //CHANGE STATE
  S = FSM[S].Next[input];
}
