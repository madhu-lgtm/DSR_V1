/*
Code Version3 :- Update Code for Testing on drone with reading pwm signal
700 steps to close and open
Code Version2 :- Add Encoder Knob for Controlling the Position and click to change direction
Code Version1  :- Check 1 Complete Rotation Cw and define min and max delay times

---------------- Control Logic -----------------
The system is used for DSR Pipe folding mechanism For Involute type DSR 
-> Need to fold and unfold the pipes upon pressing the switch on transmitter (T12 Tx)
-> For prototyping 
   - Check with any rotary encoder with click as input for speed and direction and stop/start control 
-> For Drone
  - Read pwm signal from t12 rx (ch10) 
  - For 1000us -> Pipes need to fold
  - For 2000us -> Pipes need to unfold
-------------------------------------------------

---------------- Components Used ----------------
Driver Used  = TB6600
Stepper Used = RMCS 1002 Nema23 18.9kgcm 57HS76
Gear Box = No 
Micro Step used = 1
Current Set = 3Amp
Input Volatge = 12v (8amp) 
Arduino = Nano original
t12 tx and rx

--------------------------------------------------
-------------- Connections -------------------
TB6600 to Nano
 ENA- , DIR- and PUL-  ->   gnd of arduino
 ENA+                  ->   D7
 DIR+                  ->   D8
 PUL+                  ->   D9 

T12 Reciver to nano
ch10                  ->    D10
Gnd                   ->    Gnd     
-----------------------------------------------

Created on :- 23-01-2026
Updated on :- 16-feb-2026 (Updated For Belt drive from 900 to 100)
Created by :- K Madhu Mohan Chary 
For code enquiry :- madhu@marutdrones.com 
*/

// Used Common cathode setup on driver
const byte pul_pin = 9; //pulse pin 
const byte dir_pin = 8; //direction pin
const byte ena_pin = 7; //enable pin

// Configure the stepper and driver 
int SPR = 200;// Steps per rotation ; set-> S1 = 1 ,S2 = 1, S3 = 0 | if 3Amp set S4 = 0, S5 = 1, S6 = 0
bool rotate_cw = LOW; //cw from top view i,e shaft view or belt side of stepper
bool rotate_ccw = HIGH; 
volatile int min_us = 750; //750us For Nema23 57HS76 @3amp,200SPR
volatile int max_us = 8000; // 2000us @3amp,200SPR
volatile unsigned long step ;

// Configure t12 reciver
const byte ch10_pin = 10;
const int ch10_min = 1051;
const int ch10_max = 1951;
const int ch10_mid = 1501;
const int ch10_buff = 100; //pwm reading buffer 
volatile int ch10_val = ch10_min; //initial read value

// logic
bool is_fold = HIGH;//monitor the state of pipes (fold or unfold)
int steps_to_close_or_open = 300;//Updated For Belt Folding :- 100-> 800 -> 300
int dt = 500; //program running delay

void read_pwm()
{
  ch10_val = pulseIn(ch10_pin,HIGH);

  if (ch10_val >= (ch10_min - ch10_buff) && ch10_val <= (ch10_min + ch10_buff))
  {
      digitalWrite(dir_pin,rotate_cw); //Tune based on direction
      if (is_fold == HIGH) //i.e, pipes are fold (if pipes are horizontal)
      {
        move_stepper();
        is_fold = LOW;
      }
  }
  else if (ch10_val >= (ch10_max - ch10_buff) && ch10_val <= (ch10_max + ch10_buff))
  {
      digitalWrite(dir_pin,rotate_ccw);
      if(is_fold == LOW) //i.e, pipes are un fold (if pipes are verical)
      {
        move_stepper();
        is_fold = HIGH;
      }
  }
}

void move_stepper()
{
 
  for(int step = 0; step < steps_to_close_or_open; step++)
  { 
  digitalWrite(pul_pin,HIGH);
  delayMicroseconds(max_us);
  digitalWrite(pul_pin,LOW);
  delayMicroseconds(max_us);
  }

}

void setup() {
  pinMode(pul_pin,OUTPUT);
  pinMode(dir_pin,OUTPUT);
  pinMode(ena_pin,OUTPUT);
  pinMode(ch10_pin,INPUT);

  digitalWrite(ena_pin,LOW);// Enable Stepper
  digitalWrite(dir_pin,LOW);// Set Stepper direction LOW > CW and HIGH > CCW

  Serial.begin(9600);
}

void loop() {

 read_pwm();
 delay(dt);
}
