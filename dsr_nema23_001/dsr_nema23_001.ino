/*
Code Version1  :- Check 1 Complete Rotation Cw and define min and max delay times

---------------- Control Logic -----------------
The system is used for DSR Pipe folding mechanism For Involute type DSR 
-> Need to fold and unfold the pipes upon pressing the switch on transmitter (T12 Tx)
-> For prototyping 
   - Check with any rotary encoder with click as input for speed and direction and stop/start control 

-------------------------------------------------

---------------- Components Used ----------------
Driver Used  = TB6600
Stepper Used = RMCS 1002 Nema23 18.9kgcm 57HS76
Gear Box = No 
Micro Step used = 1
Current Set = 3Amp
Input Volatge = 12v (8amp) 
Arduino = Nano original
Rotary encoder input = Hw040

--------------------------------------------------
-------------- Connections -------------------
TB6600 to Nano
 ENA- , DIR- and PUL-  ->   gnd of arduino
 ENA+                  ->   D7
 DIR+                  ->   D8
 PUL+                  ->   D9 



-----------------------------------------------

Created on :- 23-01-2026
Created by :- K Madhu Mohan Chary 
For code enquiry :- madhu@marutdrones.com 
*/

// Used Common cathode setup on driver
const byte pul_pin = 9; //pulse pin 
const byte dir_pin = 8; //direction pin
const byte ena_pin = 7; //enable pin


// Configure the stepper and driver 
int SPR = 200;// Steps per rotation > S1 = 1 ,S2 = 1, S3 = 0 | if 3Amp set S4 = 0, S5 = 1, S6 = 0
bool rotate_cw = LOW; //cw from top view i,e shaft view or belt side of stepper
bool rotate_ccw = HIGH; 
volatile int min_us = 1000; //750us For Nema23 57HS76 @3amp,200SPR
volatile int max_us = 2000; // 2000us @3amp,200SPR
volatile unsigned long step ;

void setup() {
  pinMode(pul_pin,OUTPUT);
  pinMode(dir_pin,OUTPUT);
  pinMode(ena_pin,OUTPUT);
  digitalWrite(ena_pin,LOW);// Enable Stepper
  digitalWrite(dir_pin,LOW);// Set Stepper direction LOW > CW and HIGH > CCW
}

void loop() {
  for (step = 0 ; step <= SPR; step++)
  {
    digitalWrite(pul_pin,HIGH);
    delayMicroseconds(min_us);
    digitalWrite(pul_pin,LOW);
    delayMicroseconds(min_us);
  }
}
