#include "pitches.h"
#define C 5 
#define D 6
#define E 7
#define F 8
#define G 9 
#define A 10
#define B 11
#define Cs 12
#define spk 2
#define rec 3 
#define pla 4
bool doL=1,re=1,me=1,fa=1,sol=1,la=1,ti=1,doU=1,record=1,play=1;
bool state_rec,state_pla,state=0,st=0,stp=0;
unsigned long int now=0,last1=0,last2=0,last_rec=0,last_pla,i,n;
int NOTE[8] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5};
int REC_NOTE[300] = {};
int Time[300] = {};

void setup() {
  Serial.begin(9600);
  pinMode(C,INPUT_PULLUP);
  pinMode(D,INPUT_PULLUP);
  pinMode(E,INPUT_PULLUP);
  pinMode(F,INPUT_PULLUP);
  pinMode(G,INPUT_PULLUP);
  pinMode(A,INPUT_PULLUP);
  pinMode(B,INPUT_PULLUP);
  pinMode(Cs,INPUT_PULLUP);
  pinMode(rec,INPUT_PULLUP);
  pinMode(pla,INPUT_PULLUP);
  pinMode(spk,OUTPUT);
  pinMode(13,OUTPUT);
    tone(spk,NOTE_C4); delay(200);
    noTone(spk); delay(200);
    tone(spk,NOTE_C4); delay(200);
    noTone(spk); delay(200);
}

void loop() {
    now = millis();
    AllTone();                              //Play Piano normally
    Record_Play();                          //Check if have Record(); or Play();
    
    if(state_rec == 1||state_pla == 1) digitalWrite(13,HIGH);   //Turn on GreenLight
    else digitalWrite(13,LOW);                                  //Turn off GreenLight
}

void InputButton(){
  doL = digitalRead(C); re = digitalRead(D); me = digitalRead(E); fa = digitalRead(F); 
  sol = digitalRead(G); la = digitalRead(A); ti = digitalRead(B); doU = digitalRead(Cs); 
}

void AllTone(){
  InputButton();
  if(doL == 0)tone(spk,NOTE_C4); 
  else if(re == 0)tone(spk,NOTE_D4); 
  else if(me == 0)tone(spk,NOTE_E4); 
  else if(fa == 0)tone(spk,NOTE_F4); 
  else if(sol == 0)tone(spk,NOTE_G4); 
  else if(la == 0)tone(spk,NOTE_A4); 
  else if(ti == 0)tone(spk,NOTE_B4);
  else if(doU == 0)tone(spk,NOTE_C5); 
  else noTone(spk); 
}

void Record_Play(){
  record = digitalRead(rec);
  play = digitalRead(pla);
  if(record == 0 && now - last_rec >= 300 && state_pla == 0){   //Debounce button Record
    if(state_rec == 0) {state_rec = 1;} else {state_rec = 0;}   //Change state Record to On or Off
    last_rec = now;
  }
  if(play == 0 && now - last_pla >= 300 && state_rec == 0){     //Debounce button Play
    if(state_pla == 0) {state_pla = 1;} else {state_pla = 0;}   //Change state Play to On or Off
    last_pla = now;
  }
  if(state_rec == 1){             //Check If Record state is on ...
    if(st == 0 && stp != 0){ //(*1) //This below loop need to run only one time! if not...it will delete their own record
      for(int k=1; k<300; k++){     //Loop_Clear All note in storage for next new note
        REC_NOTE[k]=0; Time[k]=0;     
      }
      st = 1;                       //Change state of this loop...
    }
      Record();                   
      n=i;                          //Count.. how many box of array was used?
      stp = 0;                      //Set state if play to 0 
  }
  if(state_pla == 1){             //Check If Play state is on ...
    st = 0;                         //Set state if record to 0 
    Play();
    stp++;       //state of play.. need it because if case of record 2 time and don't play. If don't have it. It will not delete note in storage. See in (*1)
    state_pla = 0;                //Change state back to 0 run only one time
  }
}

void Record(){
  if(doL==1 && re==1 && me==1 && fa==1 && sol==1 && la==1 && ti==1 && doU==1 ){ //If Nothing* is pressed...
    Serial.println("REC");                                          //print_Check in Record function
    last2 = now;                                                                //Reset Time of someTone_NOTE
    if(state == 0){i++; state=1;}                                               //Change state move to next array [i++] **only one time!
    REC_NOTE[i] = 0; Time[i] = now-last1;                                       //Record NOTE* and TIME Duration*
  } 
  else if(doL==0 || re==0 || me==0 || fa==0 || sol==0 || la==0 || ti==0 || doU==0){ //If Something* is pressed...
    last1 = now;                                                                //Reset Time of noTone_NOTE
    if(state == 1){i++; state=0;}                                               //Change state move to next array [i++] **only one time!
    
    if(doL == 0){REC_NOTE[i] = NOTE[0]; Time[i] = now-last2;}         //Record NOTE* and TIME Duration*
    else if(re == 0){REC_NOTE[i] = NOTE[1];  Time[i] = now-last2;}    //Record NOTE* and TIME Duration*
    else if(me == 0){REC_NOTE[i] = NOTE[2]; Time[i] = now-last2;}     //Record NOTE* and TIME Duration*
    else if(fa == 0){REC_NOTE[i] = NOTE[3]; Time[i] = now-last2;}     //Record NOTE* and TIME Duration*
    else if(sol == 0){REC_NOTE[i] = NOTE[4]; Time[i] = now-last2;}    //Record NOTE* and TIME Duration*
    else if(la == 0){REC_NOTE[i] = NOTE[5]; Time[i] = now-last2;}     //Record NOTE* and TIME Duration*
    else if(ti == 0){REC_NOTE[i] = NOTE[6]; Time[i] = now-last2;}     //Record NOTE* and TIME Duration*
    else if(doU == 0){REC_NOTE[i] = NOTE[7]; Time[i] = now-last2;}    //Record NOTE* and TIME Duration*
    Serial.print("|");                                              //print_Check something was record it
  }  
}

void Play(){
  digitalWrite(13,HIGH);                //Turn on Green Light only 1 time
  Serial.println("PLAY TONE");     //print_Check in Play function
  for(int j=1; j<n; j++){               //Loop PLAY ALL NOTE only 1 time
    Serial.println(j);             //print_Check What is the number of array now? 
    if(REC_NOTE[j]==0){
      noTone(spk); delay(Time[j]); 
    }
    else{
      tone(spk,REC_NOTE[j]); delay(Time[j]);
    }
  }
  digitalWrite(13,LOW);                 //Turn off Green Light only 1 time
  i=0;                                  //If Play, then set Record at the first position again
}
