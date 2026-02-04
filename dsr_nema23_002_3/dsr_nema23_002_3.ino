/*
Code Version1 :- Add Encoder Knob for Controlling the Position 
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
Rotary encoder input = Hw040 30pulses per 1 complete rotation

--------------------------------------------------
-------------- Connections -------------------
TB6600 to Nano
 ENA- , DIR- and PUL-  ->   gnd of arduino
 ENA+                  ->   D7
 DIR+                  ->   D8
 PUL+                  ->   D9 

Encode Knob to nano 
Clk O/p A             ->    D2
Dt  O/p B             ->    D3
Sw                    ->    D4
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


//Knob configuration
volatile byte en_clk_pin = 2;
//volatile byte en_dt_pin = 3;
volatile byte en_sw_pin = 3;
volatile float stepper_angle ;
volatile byte KSPR = 30; //knob Steps per rotation 
volatile int knob_step = 1;
volatile int req_step = 0;
volatile byte cond1 = LOW;
const int map_knob_stepper = SPR/KSPR;
volatile byte state = LOW;

void read_knob()
{
  cond1 = HIGH;
}

void read_btn()
{
  state = !state;
  digitalWrite(dir_pin,state);
}

void move_stepper()
{
      if (cond1 == HIGH)
    {
      for(int n = 0; n < 50; n++){ //map_knob_stepper
      digitalWrite(pul_pin,HIGH);
      delayMicroseconds(min_us);
      digitalWrite(pul_pin,LOW);
      delayMicroseconds(min_us);
      //Serial.println(n);
      }
      cond1 = LOW;
    }  

}

void setup() {
  pinMode(pul_pin,OUTPUT);
  pinMode(dir_pin,OUTPUT);
  pinMode(ena_pin,OUTPUT);
  digitalWrite(ena_pin,LOW);// Enable Stepper
  digitalWrite(dir_pin,LOW);// Set Stepper direction LOW > CW and HIGH > CCW

  pinMode(en_clk_pin,INPUT);
  //pinMode(en_dt_pin,INPUT);
  pinMode(en_sw_pin,INPUT);
  attachInterrupt(digitalPinToInterrupt(en_clk_pin),read_knob,CHANGE);
  attachInterrupt(digitalPinToInterrupt(en_sw_pin),read_btn,RISING);
  Serial.begin(9600);

}

void loop() {

 move_stepper();
  
}

  // for (step = 0 ; step <= SPR; step++)
  // {
  //   digitalWrite(pul_pin,HIGH);
  //   delayMicroseconds(min_us);
  //   digitalWrite(pul_pin,LOW);
  //   delayMicroseconds(min_us);
  // }
