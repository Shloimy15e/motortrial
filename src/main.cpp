#include <Arduino.h>
#include <Wire.h>

// Joystick Pins
int x_key = A0;
int y_key = A1;
int x_pos;
int y_pos;

// Motor Pins for driver a
int EN_A1 = 11; // Enable pin for first motor
int IN1a = 9;   // control pin for first motor
int IN2a = 8;   // control pin for first motor
int IN3a = 7;   // control pin for second motor
int IN4a = 6;   // control pin for second motor
int EN_B1 = 10; // Enable pin for second motor

// Motor Pins for driver b
int EN_A2 = 3; // Enable pin for first motor
int IN1b = 13; // control pin for first motor
int IN2b = 12; // control pin for first motor
int IN3b = 4;  // control pin for second motor
int IN4b = 2;  // control pin for second motor
int EN_B2 = 5; // Enable pin for second motor

// Initializing variables to store data
int motor_speed;
int motor_speed1;

void setup()
{
  Serial.begin(9600); // Starting the serial communication at 9600 baud rate
  // Initializing the motor pins as output
  pinMode(EN_A1, OUTPUT);
  pinMode(IN1a, OUTPUT);
  pinMode(IN2a, OUTPUT);
  pinMode(IN3a, OUTPUT);
  pinMode(IN4a, OUTPUT);
  pinMode(EN_B1, OUTPUT);

  pinMode(EN_A2, OUTPUT);
  pinMode(IN1b, OUTPUT);
  pinMode(IN2b, OUTPUT);
  pinMode(IN3b, OUTPUT);
  pinMode(IN4b, OUTPUT);
  pinMode(EN_B2, OUTPUT);

  // Initializng the joystick pins as input
  pinMode(x_key, INPUT);
  pinMode(y_key, INPUT);
}

void loop()
{
  x_pos = analogRead(x_key); // Reading the horizontal movement value
  y_pos = analogRead(y_key); // Reading the vertical movement value

  /**
   * If the joystick is moved in the left direction
   * then motor1 will rotate in counter clockwise direction
   * and motor2 will rotate in clockwise direction
   * and motor3 will rotate in counter clockwise direction
   * and motor4 will rotate in clockwise direction
   * If the joystick is moved in the right direction
   * then motor1 will rotate in clockwise direction
   * and motor2 will rotate in counter clockwise direction
   * and motor3 will rotate in clockwise direction
   * and motor4 will rotate in counter clockwise direction
   */

  if (y_pos < 400)
  { // Robot heads in reverse and direction is according to x_pos
    motor_speed1 = map(y_pos, 400, 0, 0, 255);
    analogWrite(EN_A1, motor_speed1);
    analogWrite(EN_A2, motor_speed1);
    analogWrite(EN_B1, motor_speed1);
    analogWrite(EN_B2, motor_speed1);
    // If the joystick is moved in the left direction
    if (x_pos < 400)
    {
      // Head left in reverse direction
      // Motor1 and Motor3 will rotate in counter clockwise direction
      // Motor2 and Motor4 will rotate in clockwise direction
      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, HIGH);
      digitalWrite(IN3a, HIGH);
      digitalWrite(IN4a, LOW);
      digitalWrite(IN1b, LOW);
      digitalWrite(IN2b, HIGH);
      digitalWrite(IN3b, HIGH);
      digitalWrite(IN4b, LOW);
    }
    // If the joystick is in the center
    else if (x_pos > 400 && x_pos < 600)
    {
      // Head straight in reverse direction
      digitalWrite(IN1a, HIGH);
      digitalWrite(IN2a, LOW);
      digitalWrite(IN3a, HIGH);
      digitalWrite(IN4a, LOW);
      digitalWrite(IN1b, HIGH);
      digitalWrite(IN2b, LOW);
      digitalWrite(IN3b, HIGH);
      digitalWrite(IN4b, LOW);
    }
    else if (x_pos > 600)
    {
      // Head right in reverse direction
      // Motor1 and Motor3 will rotate in clockwise direction
      // Motor2 and Motor4 will rotate in counter clockwise direction
      digitalWrite(IN1a, HIGH);
      digitalWrite(IN2a, LOW);
      digitalWrite(IN3a, LOW);
      digitalWrite(IN4a, HIGH);
      digitalWrite(IN1b, HIGH);
      digitalWrite(IN2b, LOW);
      digitalWrite(IN3b, LOW);
      digitalWrite(IN4b, HIGH);
    }
  }

  else if (y_pos > 400 && y_pos < 600)
  {
    // if the joystick is moved in the left direction
    if (x_pos < 400)
    {
      // motor1 and motor3 will rotate in counter clockwise direction
      // motor2 and motor4 will rotate in clockwise direction
      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, HIGH);
      digitalWrite(IN3a, HIGH);
      digitalWrite(IN4a, LOW);
      digitalWrite(IN1b, LOW);
      digitalWrite(IN2b, HIGH);
      digitalWrite(IN3b, HIGH);
      digitalWrite(IN4b, LOW);
      motor_speed1 = map(x_pos, 400, 0, 0, 255);
      analogWrite(EN_A1, motor_speed1);
      analogWrite(EN_A2, motor_speed1);
      analogWrite(EN_B1, motor_speed1);
      analogWrite(EN_B2, motor_speed1);
    }
    // if the joystick is moved in the right direction
    else if (x_pos > 600)
    {
      // motor1 and motor3 will rotate in clockwise direction
      // motor2 and motor4 will rotate in counter clockwise direction
      digitalWrite(IN1a, HIGH);
      digitalWrite(IN2a, LOW);
      digitalWrite(IN3a, LOW);
      digitalWrite(IN4a, HIGH);
      digitalWrite(IN1b, HIGH);
      digitalWrite(IN2b, LOW);
      digitalWrite(IN3b, LOW);
      digitalWrite(IN4b, HIGH);
      motor_speed1 = map(x_pos, 600, 1023, 0, 255);
      analogWrite(EN_A1, motor_speed1);
      analogWrite(EN_A2, motor_speed1);
      analogWrite(EN_B1, motor_speed1);
      analogWrite(EN_B2, motor_speed1);
    }
    else
    {
      // If the joystick is in the center
      digitalWrite(IN3a, LOW);
      digitalWrite(IN4a, LOW);
      digitalWrite(IN3b, LOW);
      digitalWrite(IN4b, LOW);
      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, LOW);
      digitalWrite(IN1b, LOW);
      digitalWrite(IN2b, LOW);
    }
  }

  else if (y_pos > 600)
  { // Robot heads in forward direction and direction is according to x_pos
    motor_speed1 = map(y_pos, 600, 1023, 0, 255);
    // all the motors will rotate in clockwise direction
    analogWrite(EN_A1, motor_speed1);
    analogWrite(EN_A2, motor_speed1);
    analogWrite(EN_B2, motor_speed1);
    analogWrite(EN_B1, motor_speed1);

    // If the joystick is moved in the left direction
    if (x_pos < 400)
    {
      // Head left in forward direction
      // Motor1 and Motor3 will rotate in counter clockwise direction
      // Motor2 and Motor4 will rotate in clockwise direction
      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, HIGH);
      digitalWrite(IN3a, HIGH);
      digitalWrite(IN4a, LOW);
      digitalWrite(IN1b, LOW);
      digitalWrite(IN2b, HIGH);
      digitalWrite(IN3b, HIGH);
      digitalWrite(IN4b, LOW);  
    }
    // If the joystick is in the center
    else if (x_pos > 400 && x_pos < 600)
    {
      // Head straight in forward direction
      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, HIGH);
      digitalWrite(IN3a, LOW);
      digitalWrite(IN4a, HIGH);
      digitalWrite(IN1b, LOW);
      digitalWrite(IN2b, HIGH);
      digitalWrite(IN3b, LOW);
      digitalWrite(IN4b, HIGH);
    }
    // If the joystick is moved in the right direction
    else if (x_pos > 600)
    {
      // Head right in forward direction
      // Motor1 and Motor3 will rotate in clockwise direction
      // Motor2 and Motor4 will rotate in counter clockwise direction
      digitalWrite(IN1a, HIGH);
      digitalWrite(IN2a, LOW);
      digitalWrite(IN3a, LOW);
      digitalWrite(IN4a, HIGH);
      digitalWrite(IN1b, HIGH);
      digitalWrite(IN2b, LOW);
      digitalWrite(IN3b, LOW);
      digitalWrite(IN4b, HIGH);
    }
  }
}