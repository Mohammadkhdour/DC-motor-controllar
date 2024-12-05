#include <LiquidCrystal.h>

// Define LCD pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Motor control pins
#define ENCA 2  // Encoder A
#define ENCB 3  // Encoder B
#define PWM 5
#define IN2 6
#define IN1 7
#define POT A0

 int posi = 0; // Motor position (volatile for ISR)

// Setup
void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Setup encoder pins as input
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(POT, INPUT);
  
  // Attach interrupt to read encoder position on rising edge of ENCA
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  
  // Setup motor control pins
  pinMode(PWM, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  

  // Initialize LCD
  lcd.begin(16, 2);  // 16 columns, 2 rows
  lcd.setCursor(0, 0);  // Move cursor to first row
  lcd.print("Motor Position:");  // Display label
}

// Loop
void loop() {
  // Read the potentiometer (simulating motor control position)
  int pot = analogRead(POT);
  int half = 512;

  // Display position on LCD
  lcd.setCursor(0, 1);  // Move cursor to second row
  lcd.print("Pos: ");
  lcd.print(pot);  // Display motor position
Serial.println(pot);
  // Control motor based on potentiometer value
  if (pot > half + 120) {
    Serial.println("Forward");
    setMotor(1, pot - half, PWM, IN1, IN2);  // Move motor forward
  } else if (pot < half - 120) {
    Serial.println("Backward");
    setMotor(-1, half - pot, PWM, IN1, IN2);  // Move motor backward
  } else {
    Serial.println("Stopped");
    setMotor(0, 25, PWM, IN1, IN2);  // Stop the motor
  }
}

// Motor control function
void setMotor(int dir, int pwmVal, int pwm, int in1, int in2) {
  analogWrite(pwm, pwmVal);
  if (dir == 1) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (dir == -1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

// Encoder interrupt handler
void readEncoder() {
  int b = digitalRead(ENCB);
  if (b < 0) {
    posi++;  // Increment position if encoder is in the right direction
  } else {
    posi--;  // Decrement position if encoder is in the opposite direction
  }
}

