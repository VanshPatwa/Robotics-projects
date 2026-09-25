//------------SPEED-----------------------------------------------------
int enA = 6;//right motors
int enB = 5;//left motors
int speed=100;
//------------MOTORS----------------------------------------------------
//POSITIVE VOLTAGE TO POSITIVE VARAIBLE => FORWARD (ROTATION)
int right_positive = 7;//IN1 ----- TERMINALS FOR RIGHT MOTOR
int right_negative = 8;//IN2
int left_negative = 10;//IN3 ------ TERMINALS FOR LEFT MOTOR
int left_positive = 9;//IN4
//------------SENSORS---------------------------------------------------
int left_most_sensor = 13;//left sensor pair
int left_sensor = 12;
int middle_sensor = 11;// extended sensor 
int right_sensor = 3;// right sensor pair
int right_most_sensor = 2;
//------------READINGS--------------------------------------------------
int blak = 1;// REFLECT and NOT REFLECT values
int whit = 0;
int l=0,lm=0,m=0,rm=0,r=0;
float kp =17, kd =48, ki = 0;                   //-------- Values to be fine tuned
float per = 0;
float er = 0;
float I = 0;
float adj =0;
void setup()//----------------------------------------------------------
{
pinMode(enA,OUTPUT);
pinMode(right_positive,OUTPUT);
pinMode(right_negative,OUTPUT);
pinMode(enB,OUTPUT);
pinMode(left_positive,OUTPUT);
pinMode(left_negative,OUTPUT);
pinMode(middle_sensor,INPUT);
pinMode(left_sensor,INPUT);
pinMode(left_most_sensor,INPUT);
pinMode(right_sensor,INPUT);
pinMode(right_most_sensor,INPUT);
}
void move(int lp,int ln,int rn,int rp){
  digitalWrite(left_positive,lp);
  digitalWrite(left_negative,ln);
  digitalWrite(right_negative,rn);
  digitalWrite(right_positive,rp);
  }
void read(){
  l=!digitalRead(left_most_sensor);
  lm=!digitalRead(left_sensor);
  m=!digitalRead(middle_sensor);
  rm=!digitalRead(right_sensor);
  r=!digitalRead(right_most_sensor);
}
void check() {
   // Save the last error before calculating the new one

  // --- LINE IS TO THE RIGHT (Positive Error) ---
  if      (l==whit && lm==whit && m==whit && rm==whit && r==blak) er = 4;   // 11110
  else if (l==whit && lm==whit && m==whit && rm==blak && r==blak) er = 3.5; // 11100
  else if (l==whit && lm==whit && m==whit && rm==blak && r==whit) er = 3;   // 11101
  else if (l==whit && lm==whit && m==blak && rm==blak && r==blak) er = 2.5; // 11000
  else if (l==whit && lm==whit && m==blak && rm==blak && r==whit) er = 2;   // 11001
  else if (l==whit && lm==blak && m==blak && rm==blak && r==blak) er = 1.5; // 10000
  else if (l==whit && lm==blak && m==whit && rm==blak && r==blak) er = 0;   // 10100 (Noise - treat as Right)
  else if (l==whit && lm==whit && m==blak && rm==whit && r==blak) er = 0; // 11010 (Gap - treat as Right)
  else if (l==whit && lm==blak && m==whit && rm==whit && r==blak) er = 0;   // 10110
  else if (l==whit && lm==blak && m==blak && rm==whit && r==blak) er = 0;   // 10010

  // --- CENTER CASES (Zero Error) ---
  else if (l==whit && lm==whit && m==blak && rm==whit && r==whit) er = 0;   // 11011 (Perfect Center)
  else if (l==whit && lm==blak && m==blak && rm==blak && r==whit) er = -2.1;   // 10001 (Thick Line)
  else if (l==blak && lm==blak && m==blak && rm==blak && r==blak) er = -4;   // 00000 (Cross/Intersection)
  else if (l==blak && lm==whit && m==whit && rm==whit && r==blak) er = 0;   // 01110 (Edges only)
  else if (l==blak && lm==blak && m==whit && rm==blak && r==blak) er = 0;   // 00100 (Double line)
  else if (l==blak && lm==whit && m==blak && rm==whit && r==blak) er = 0;   // 01010 (Stripes)

  // --- LINE IS TO THE LEFT (Negative Error) ---
  else if (l==whit && lm==blak && m==blak && rm==whit && r==whit) er = -2;  // 10011
  else if (l==blak && lm==blak && m==blak && rm==whit && r==whit) er = -2.5;// 00011
  else if (l==whit && lm==blak && m==whit && rm==whit && r==whit) er = -3;  // 10111
  else if (l==blak && lm==blak && m==whit && rm==whit && r==whit) er = -3.5;// 00111
  else if (l==blak && lm==whit && m==whit && rm==whit && r==whit) er = -4;  // 01111
  else if (l==blak && lm==blak && m==blak && rm==blak && r==whit) er = 1.5;// 00001
  else if (l==blak && lm==whit && m==blak && rm==whit && r==whit) er = 0; // 01011 (Gap - treat as Left)
  else if (l==blak && lm==whit && m==whit && rm==blak && r==whit) er = 0;   // 01101 (Noise - treat as Left)
  else if (l==blak && lm==blak && m==whit && rm==blak && r==whit) er = 0;   // 00101
  else if (l==blak && lm==whit && m==blak && rm==blak && r==whit) er = 0;   // 01001

  // --- REMAINING "NOISY" COMBINATIONS ---
  else if (l==blak && lm==blak && m==blak && rm==whit && r==blak) er = -1;  // 00010
  else if (l==blak && lm==whit && m==blak && rm==blak && r==blak) er = 1;   // 01000
  else if (l==whit && lm==blak && m==blak && rm==blak && r==whit) er = -1;   // 10001
  else if (l==blak && lm==blak && m==whit && rm==whit && r==blak) er = 2;  // 00110
  else if (l==blak && lm==whit && m==whit && rm==blak && r==blak) er = -2;   // 01100

  // --- LOST LINE (All White) ---
  else if (l==whit && lm==whit && m==whit && rm==whit && r==whit) {
    if (per > 0) er = 5;       // If we last saw it on the right, turn harder right
    else if (per < 0) er = -5;  // If we last saw it on the left, turn harder left
    else er = 0;
  }
}
void set_speed(int left_speed,int right_speed){
  analogWrite(enA,(right_speed+8.8));
  analogWrite(enB,left_speed);
}
void loop() {
  // put your main code here, to run repeatedly:
  read();
  per = er;
  check();
  I = I + er;
  adj = (kp*er) + (ki*I) + (kd*(er-per));       //---------- Adjustment PID
  if(er<=5){
    set_speed(speed+adj,speed-adj);           //-------------- Everything else (forward and slight turns)
    move(1,0,0,1);
  }
  
}v
