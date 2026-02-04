/*
  Updated: 23-01-2026
  Author: K Madhu Mohan Chary (Modified for Stability)
  System: DSR Pipe Folding - TB6600 + Nema23 + HW040
*/

// Driver Pins
const byte pul_pin = 9; 
const byte dir_pin = 8; 
const byte ena_pin = 7; 

// Encoder Pins
const byte en_clk_pin = 2;
const byte en_dt_pin = 3;
const byte sw_pin = 4;

// Stepper Settings
const int SPR = 200;           // Steps per rotation (Driver S1,S2,S3 configuration)
const int min_us = 800;        // Pulse width (Adjust for speed)
const int steps_per_click = 7; // Approx 200/30 = 6.66. Set to 7 for responsiveness.

// Control Variables
volatile int knob_delta = 0;   // Stores pending steps to move
volatile unsigned long last_interrupt_time = 0; 

void update_encoder() {
  unsigned long interrupt_time = millis();
  
  // Simple Debounce: ignore interrupts closer than 5ms
  if (interrupt_time - last_interrupt_time > 5) {
    // Read DT pin to determine direction relative to CLK
    if (digitalRead(en_dt_pin) != digitalRead(en_clk_pin)) {
      digitalWrite(dir_pin, LOW);  // CW
      knob_delta++;
    } else {
      digitalWrite(dir_pin, HIGH); // CCW
      knob_delta--;
    }
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  pinMode(pul_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(ena_pin, OUTPUT);
  
  digitalWrite(ena_pin, LOW); // Enable Driver (Active Low)
  
  pinMode(en_clk_pin, INPUT_PULLUP);
  pinMode(en_dt_pin, INPUT_PULLUP);
  pinMode(sw_pin, INPUT_PULLUP);

  // Trigger on CHANGE to catch both edges for better resolution
  attachInterrupt(digitalPinToInterrupt(en_clk_pin), update_encoder, RISING);
  
  Serial.begin(9600);
  Serial.println("System Ready - DSR Pipe Mechanism");
}

void loop() {
  // Check if encoder has been turned
  if (knob_delta != 0) {
    
    // Move the stepper by the multiplier
    for (int n = 0; n < steps_per_click; n++) {
      digitalWrite(pul_pin, HIGH);
      delayMicroseconds(min_us);
      digitalWrite(pul_pin, LOW);
      delayMicroseconds(min_us);
    }

    // Safely decrement the delta
    if (knob_delta > 0) knob_delta--;
    else knob_delta++;
    
    // Optional Debug
    // Serial.print("Remaining Delta: "); Serial.println(knob_delta);
  }
}
