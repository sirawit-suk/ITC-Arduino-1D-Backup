#include "LedControl.h"

//////////////////////////////////// DEFINE VARIABLE //////////////////////////////////////////////
LedControl lc=LedControl(11,13,10,4);  // DIN,CLK,CS,Number of LED Module

#define up_button 2
#define down_button 4
#define left_button 5
#define right_button 3
#define start_button 6
#define select_button 7
#define joystick_button 8
#define joystick_axis_x A0
#define joystick_axis_y A1

int button[] = {up_button, down_button, left_button, right_button, start_button, select_button, joystick_button};
unsigned int intensity = 7;
unsigned long now;
unsigned long last;
unsigned long last2;

bool UP = 1;
bool DOWN = 1;
bool LEFT = 1;
bool RIGHT = 1;
bool Tempstate = 0;
bool enemyTempstate = 0;

int main_state = 0;
int joy_x = 0;
int joy_y = 0;
int x_ship;
int y_ship;
int x_enemy;
int y_enemy;

#define Num_bullet 10     //define how many bullet can use

int HP = 8;
int enemyHP = 8;
int x_bullet[Num_bullet];
int y_bullet[Num_bullet];
int x_enemybullet[Num_bullet];
int y_enemybullet[Num_bullet];
int bullet_state[Num_bullet] = {0};
int bullet_enemystate[Num_bullet] = {0};

//////////////////////////////////// SET-UP //////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  for(int address = 0; address < 4; address++) {  //Set up for 4 Matrix 
    lc.shutdown(address,false);
    lc.setIntensity(address,intensity);
    lc.clearDisplay(address);
  }
  for(int i = 0; i < 7; i++) {  //Set up All button on Joystick
    pinMode(button[i],INPUT);
    digitalWrite(button[i],HIGH);
  }
  randomSeed(analogRead(A0));
  x_ship = 4;   //set x,y ship
  y_ship = 32;
  x_enemy = 4;
  y_enemy = 1;
}


////////////////////////////////// MAIN LOOP //////////////////////////////////////////////
void loop() { 
  Input_system();
  
  switch (main_state){
    case 0: Setup_interface();    //Main menu
            if(UP == 0 || DOWN == 0 || LEFT == 0 || RIGHT == 0){  // If something was pressed...
              clear_display();
              Pre_interface();
              clear_display();

              Set_newgame();
              main_state = 1;
            }
            break;
            
    case 1: Control_ship();       //MY SHIP
            Check_bullet();
  
            Control_enemy();      //ENEMY
            Check_enemybullet();

            Check_collision();    //CHECK if have collision
            if(HP == 24 || enemyHP == 24){
              clear_display();
              main_state = 2;
            }
            break;
              
    case 2: End_interface();      //Tell Win or Lose
            main_state = 0;
            break;
  }
  
  delay(50);
}

////////////////////////////////// GAME SYSTEM //////////////////////////////////////////////
void Input_system(){
  now = millis();
  joy_x = map(analogRead(joystick_axis_x),0,1000,-1,1); 
  joy_y = map(analogRead(joystick_axis_y),0,1000,-1,1);
  UP    = digitalRead(up_button);
  DOWN  = digitalRead(down_button);
  LEFT  = digitalRead(left_button);
  RIGHT = digitalRead(right_button);
}
void Control_ship(){
  draw_ship(x_ship,y_ship); 
  if(joy_x == -1) {if(x_ship!=2)    {delete_ship(x_ship,y_ship);  draw_ship(--x_ship,y_ship);}  }     //LEFT
  if(joy_x == 1)  {if(x_ship!=7)    {delete_ship(x_ship,y_ship);  draw_ship(++x_ship,y_ship);}  }     //RIGHT
  if(joy_y == -1) {if(y_ship!=32)   {delete_ship(x_ship,y_ship);  draw_ship(x_ship,++y_ship);}  }     //DOWN
  if(joy_y == 1)  {if(y_ship!=26)   {delete_ship(x_ship,y_ship);  draw_ship(x_ship,--y_ship);}  }     //UP
}
void Control_enemy(){
  int randNum = random(0,10);
  draw_enemy(x_enemy,y_enemy);
  if(randNum == 3)  {if(x_enemy!=2)  {delete_enemy(x_enemy,y_enemy);  draw_enemy(--x_enemy,y_enemy);}  } //RANDOM LEFT
  if(randNum == 7)  {if(x_enemy!=7)  {delete_enemy(x_enemy,y_enemy);  draw_enemy(++x_enemy,y_enemy);}  } //RANDOM RIGHT 
  if(randNum == 4)  {if(y_enemy!=7)  {delete_enemy(x_enemy,y_enemy);  draw_enemy(x_enemy,++y_enemy);}  } //RANDOM DOWN
  if(randNum == 8)  {if(y_enemy!=1)  {delete_enemy(x_enemy,y_enemy);  draw_enemy(x_enemy,--y_enemy);}  } //RANDOM UP
}
void Set_newgame(){
  setup_healthbar();
  set_bulletstate();
  set_enemybulletstate();
  draw_ship(x_ship,y_ship);  //set draw_ship
  draw_enemy(x_enemy,y_enemy); //set draw_enemy
}
void Check_collision(){
  for(int i = 0; i < Num_bullet; i++){
    if(y_bullet[i] < 30){
      for(int j = 0; j < Num_bullet; j++){
        if(y_enemybullet[j] >2){
            if(y_bullet[i] == y_enemybullet[j] && x_bullet[i] == x_enemybullet[j]){
              bullet_state[i] = 0;
              bullet_enemystate[j] = 0;
              delete_plot(x_bullet[i],y_bullet[i]);
              delete_plot(x_enemybullet[j],y_enemybullet[j]);
              x_bullet[i] = 0; y_bullet[i] = 0;
              x_enemybullet[j] = 0,y_enemybullet[j] = 0;
            }
            else if(y_bullet[i] == 1+y_enemybullet[j] &&  x_bullet[i] == x_enemybullet[j]){
              bullet_state[i] = 0;
              bullet_enemystate[j] = 0;
              delete_plot(x_bullet[i],y_bullet[i]);
              delete_plot(x_enemybullet[j],y_enemybullet[j]);
              x_bullet[i] = 0; y_bullet[i] = 0;
              x_enemybullet[j] = 0; y_enemybullet[j] = 0;
            }
         }
       }
    }
  }
}
////////////////////////////////// MY SHIP //////////////////////////////////////////////
void draw_ship(int x, int y){  
  plot(x,y);
  plot(x,y-1);
  plot(x-1,y);
  plot(x+1,y);
}
void delete_ship(int x, int y){
  delete_plot(x,y);
  delete_plot(x,y-1);
  delete_plot(x-1,y);
  delete_plot(x+1,y);
}

////////////////////////////////// ENEMY SHIP //////////////////////////////////////////////
void draw_enemy(int x, int y){
  plot(x,y);
  plot(x+1,y);
  plot(x-1,y);
  plot(x,y+1);           
}
void delete_enemy(int x, int y){
  delete_plot(x,y);
  delete_plot(x-1,y);
  delete_plot(x+1,y);
  delete_plot(x,y+1);                   
}

////////////////////////////////// MY BULLETS //////////////////////////////////////////////
void Check_bullet(){
  if(RIGHT == 0){ 
    for(int i = 0; i < Num_bullet ; i++ ){ // Check Bullet
      if(bullet_state[i] == 0){
        bullet_state[i] = 1;
        set_bullet(i); 
        break;
      }
    }
  } 
  for(int i = 0 ; i < Num_bullet ; i++){
    if(bullet_state[i] == 1){
      delete_bullet(x_bullet[i],y_bullet[i]);
      if(x_bullet[i] == x_enemy && y_bullet[i] == y_enemy+1 || 
         x_bullet[i] == x_enemy-1 && y_bullet[i] == y_enemy || 
         x_bullet[i] == x_enemy+1 && y_bullet[i] == y_enemy){
        enemyHP++;
        Health_bar();
        bullet_state[i] = 0;
        x_bullet[i] = 0; y_bullet[i] = 0; 
        Tempstate = 1;
      }
      if(y_bullet[i]!=0 && Tempstate == 0){
        draw_bullet(x_bullet[i],--y_bullet[i]);
      }
      else{
        Tempstate = 0;
        bullet_state[i] = 0;
        x_bullet[i] = 0; y_bullet[i] = 0;
      }
    }
  }
}
void set_bulletstate(){
  for(int i = 0; i < Num_bullet; i++){
    bullet_state[i] = 0;
  }
}
void set_bullet(int i){
  x_bullet[i] = x_ship; 
  y_bullet[i] = y_ship-2;
}
void draw_bullet(int x,int y){
  plot(x,y);
}
void delete_bullet(int x,int y){
  delete_plot(x,y);
}

////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////
void Check_enemybullet(){
  int randNum = random(0,7);
  if(randNum == 4){ 
    for(int i = 0; i < Num_bullet ; i++ ){ // Check Bullet
      if(bullet_enemystate[i] == 0){
        bullet_enemystate[i] = 1;
        set_enemybullet(i); 
        break;
      }
    }
  } 
  for(int i = 0 ; i < Num_bullet ; i++){
    if(bullet_enemystate[i] == 1){
      delete_enemybullet(x_enemybullet[i],y_enemybullet[i]);
      if(x_enemybullet[i] == x_ship && y_enemybullet[i] == y_ship-1 ||
         x_enemybullet[i] == x_ship-1 && y_enemybullet[i] == y_ship ||
         x_enemybullet[i] == x_ship+1 && y_enemybullet[i] == y_ship){
        HP++;
        Health_bar();
        bullet_enemystate[i] = 0;
        x_enemybullet[i] = 0; y_enemybullet[i] = 0;
        enemyTempstate = 1; 
      }
      if(y_enemybullet[i]!=32 && enemyTempstate == 0){
        draw_enemybullet(x_enemybullet[i],++y_enemybullet[i]);
      }
      else{
        enemyTempstate = 0;
        bullet_enemystate[i] = 0;
        x_enemybullet[i] = 0; y_enemybullet[i] = 0; 
      }
    }
  }
}
void set_enemybulletstate(){
  for(int i = 0; i < Num_bullet; i++){
    bullet_enemystate[i] = 0;
  }
}
void set_enemybullet(int i){ //randNum = random(1,7);
  x_enemybullet[i] = x_enemy; 
  y_enemybullet[i] = y_enemy+2;
}
void draw_enemybullet(int x,int y){
  plot(x,y);
}
void delete_enemybullet(int x,int y){
  delete_plot(x,y);
}

////////////////////////////////// INTERFACE HEALTH BAR //////////////////////////////////////////////
void setup_healthbar(){
  HP = 8;
  enemyHP = 8;
  for(int y = HP+1; y <= 24 ; y++){        //LEFT SIDE for Me
    plot(1,y);
  }
  for(int y = enemyHP+1; y <= 24 ; y++){    //RIGHT SIDE for Enemy
    plot(8,y);
  }
}
void Health_bar(){
    delete_plot(1,HP);
    delete_plot(8,enemyHP);
}

//////////////////////////////////// PLOT SET LED ON/OFF //////////////////////////////////////////////
void plot(int x,int y){ //8,32
  int address;
  int row = 8-x; //0-7      
  int col = (y-1)%8; //0-7   
  if(y <= 8){address = 3;} 
  else if(y <= 16){address = 2;}
  else if(y <= 24){address = 1;}
  else if(y <= 32){address = 0;}
  lc.setLed(address,row,col,true);
}
void delete_plot(int x,int y){ //8,32
  int address;
  int row = 8-x; //0-7 
  int col = (y-1)%8; //0-7
  if(y <= 8){address = 3;}
  else if(y <= 16){address = 2;}
  else if(y <= 24){address = 1;}
  else if(y <= 32){address = 0;}
  lc.setLed(address,row,col,false);
}

//////////////////////////////////// ADD ON FUNCTION //////////////////////////////////////////////
void clear_display() { //clear All 4 screen
  for (int address = 0; address < 4; address++) { 
    lc.clearDisplay(address);
  }
}
void plot_display(){
  for(int i = 1; i<=8; i++){
    for(int j = 1; j<=32; j++){
      plot(i,j);
      delay(1);
    }
  }
  for(int i = 1; i<=8; i++){
    for(int j = 1; j<=32; j++){
      delete_plot(i,j);
      delay(1);
    }
  }
}

////////////////////////////////// MENU INTERFACE ///////////////////////////////////////////////////
void Setup_interface(){
    // P 
  plot(2,2);  plot(2,3);  plot(2,4);
  plot(2,5);  plot(3,2);  plot(3,4);
  plot(4,2);  plot(4,3);  plot(4,4);

  // R
  plot(2,7);  plot(2,8);  plot(2,9);
  plot(2,10); plot(3,7);  plot(3,9);
  plot(4,7);  plot(4,8);  plot(4,9);  
  plot(5,10);

  // E
  plot(2,12); plot(2,13); plot(2,14);
  plot(2,15); plot(2,16); plot(3,12);
  plot(3,14); plot(3,16); plot(4,12);
  plot(4,14); plot(4,16);

  // S
  plot(2,18); plot(2,19); plot(2,20);
  plot(2,22); plot(3,18); plot(3,20);
  plot(3,22); plot(4,18); plot(4,20);
  plot(4,21); plot(4,22);

  // S
  plot(2,24); plot(2,25); plot(2,26);
  plot(2,28); plot(3,24); plot(3,26);
  plot(3,28); plot(4,24); plot(4,26);
  plot(4,27); plot(4,28);

  // B
  plot(6,12); plot(6,13); plot(6,14);
  plot(6,15); plot(6,16); plot(7,12);
  plot(7,14); plot(7,16); plot(8,12);
  plot(8,13); plot(8,15); plot(8,16);

  // T
  plot(6,18); plot(7,18); plot(7,19);
  plot(7,20); plot(7,21); plot(7,22); 
  plot(8,18);

  if(now - last >= 500 ){
  //////////////////////////////////// Arrow 1
  plot(7,2);  plot(7,3);  plot(7,4);
  plot(7,5);  plot(7,6);  plot(7,7);
  plot(7,8);  plot(7,9);  plot(6,8);
  plot(8,8);
  delete_plot(6,9); delete_plot(8,9);
  delete_plot(6,9); delete_plot(8,9);
  delete_plot(7,10);
  last = now;
  }
  else {
  //////////////////////////////////// Arrow(ani.) 1
  plot(7,3);  plot(7,4);  plot(7,5);
  plot(7,6);  plot(7,7);  plot(7,8);
  plot(7,9);  plot(7,10); plot(6,9);
  plot(8,9);
  delete_plot(6,8); delete_plot(8,8);
  delete_plot(7,2);
  }
  if(now - last2 >= 500){
  //////////////////////////////////// Arrow 2
  plot(7,25); plot(7,26); plot(7,27);
  plot(7,28); plot(7,29); plot(7,30);
  plot(6,26); plot(8,26); plot(7,31);
  delete_plot(6,25);  delete_plot(8,25);
  delete_plot(6,25);  delete_plot(8,25);
  delete_plot(7,24);
  last2 = now;
  }
  else{
  //////////////////////////////////// Arrow(Ani.) 2
  plot(7,24); plot(7,25); plot(7,26);
  plot(7,27); plot(7,28); plot(7,29);
  plot(6,25); plot(8,25);
  delete_plot(6,26);  delete_plot(8,26);
  delete_plot(7,31);
  }
}
void Pre_interface(){
  // 3
  plot(2,10); plot(2,11); plot(2,13); plot(2,14); 
  plot(2,16); plot(2,17); plot(3,10); plot(3,11);
  plot(3,13); plot(3,14); plot(3,16); plot(3,17);
  plot(4,10); plot(4,11); plot(4,13); plot(4,14); 
  plot(4,16); plot(4,17); plot(5,10); plot(5,11);
  plot(5,13); plot(5,14); plot(5,16); plot(5,17);
  plot(6,10); plot(6,11); plot(6,12); plot(6,13);
  plot(6,14); plot(6,15); plot(6,16); plot(6,17);
  plot(7,10); plot(7,11); plot(7,12); plot(7,13);
  plot(7,14); plot(7,15); plot(7,16); plot(7,17);
  delay(1000);
  clear_display();
  delay(100);

  // 2
  plot(2,10); plot(2,11); plot(2,13); plot(2,14);
  plot(2,16); plot(2,17); plot(3,10); plot(3,11);
  plot(3,13); plot(3,14); plot(3,16); plot(3,17);
  plot(4,10); plot(4,11); plot(4,13); plot(4,14);
  plot(4,16); plot(4,17); plot(5,10); plot(5,11);
  plot(5,13); plot(5,14); plot(5,16); plot(5,17);
  plot(6,10); plot(6,11); plot(6,12); plot(6,13);
  plot(6,14); plot(6,16); plot(6,17); plot(7,10);
  plot(7,11); plot(7,12); plot(7,13); plot(7,14);
  plot(7,16); plot(7,17); plot(2,15); plot(3,15);
  delay(1000);
  clear_display();
  delay(100);
  
  // 1
  plot(4,10); plot(4,11); plot(4,12); plot(4,13);
  plot(4,14); plot(4,15); plot(4,16); plot(4,17);
  plot(5,10); plot(5,11); plot(5,12); plot(5,13);
  plot(5,14); plot(5,15); plot(5,16); plot(5,17);
  delay(1000);
  clear_display();
  delay(100);
}
void Win(){
  // W
  plot(2,2);  plot(2,3);  plot(2,4);  plot(2,5);
  plot(3,6);  plot(3,7);  plot(4,4);  plot(4,5);
  plot(5,6);  plot(5,7);  plot(6,2);  plot(6,3);
  plot(6,4);  plot(6,5);

  // I
  plot(2,10); plot(2,15); plot(3,10); plot(3,15);
  plot(4,10); plot(4,11); plot(4,12); plot(4,13);
  plot(4,14); plot(4,15); plot(5,10); plot(5,15);
  plot(6,10); plot(6,15);

  //N
  plot(2,18); plot(2,19); plot(2,20); plot(2,21);
  plot(2,22); plot(2,23); plot(3,19); plot(3,20);
  plot(4,20); plot(4,21); plot(5,21); plot(5,22);
  plot(6,18); plot(6,19); plot(6,20); plot(6,21);
  plot(6,22); plot(6,23);
}
void Lose(){
  //L
  plot(2,2);  plot(2,3);  plot(2,4);  plot(2,5);
  plot(2,6);  plot(2,7);  plot(3,7);  plot(4,7);
  plot(5,7);  plot(6,7);  plot(7,7);

  //O
  plot(2,10); plot(2,11); plot(2,12); plot(2,13);
  plot(2,14); plot(2,15); plot(3,10); plot(3,15);
  plot(4,10); plot(4,15); plot(5,10); plot(5,15);
  plot(6,10); plot(6,15); plot(7,10); plot(7,11);
  plot(7,12); plot(7,13); plot(7,14); plot(7,15);

  //S
  plot(2,18); plot(2,19); plot(2,20); plot(2,22);
  plot(3,18); plot(3,20); plot(3,22); plot(4,18);
  plot(4,20); plot(4,22); plot(5,18); plot(5,20);
  plot(5,22); plot(6,18); plot(6,20); plot(6,22);
  plot(7,18); plot(7,20); plot(7,21); plot(7,22);

  //S
  plot(2,26); plot(2,27); plot(2,28); plot(2,30);
  plot(3,26); plot(3,28); plot(3,30); plot(4,26);
  plot(4,28); plot(4,30); plot(5,26); plot(5,28);
  plot(5,30); plot(6,26); plot(6,28); plot(6,30);
  plot(7,26); plot(7,28); plot(7,29); plot(7,30);  
}
void End_interface(){
  plot_display();
  delay(200);
  if(HP == 24){
    for(int i = 0; i < 4; i++){ //Health reach 0 
      Lose(); 
      delay(700); 
      clear_display(); 
      delay(300);
    }
  }   
  else if(enemyHP == 24){  //enemyHealth reach 0
    for(int i = 0; i < 4; i++){ //Health reach 0 
      Win(); 
      delay(700); 
      clear_display(); 
      delay(300);
    }
  }
}
