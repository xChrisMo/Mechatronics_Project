#include <Servo.h>

// Motor control pins
const int RIGHT_SPEED_PIN = 3;
const int RIGHT_DIR_PIN1 = 12;
const int RIGHT_DIR_PIN2 = 11;
const int LEFT_SPEED_PIN = 6;
const int LEFT_DIR_PIN1 = 7;
const int LEFT_DIR_PIN2 = 8;

// Ultrasonic sensor pins
const int ECHO_PIN = 2;
const int TRIG_PIN = 10;

// Other pins
const int SERVO_PIN = 9;
const int BUZZER_PIN = 13;

// Speed constants
const int FAST_SPEED = 250;
const int NORMAL_SPEED = 120;
const int TURN_SPEED = 200;
const int REVERSE_SPEED = 255;

// Distance limits
const int FRONT_DISTANCE_LIMIT = 30;
const int SIDE_DISTANCE_LIMIT = 30;

// Timing constants
const int TURN_TIME = 250;
const int REVERSE_TIME = 300;

Servo headServo;

void setupMotors() {
  pinMode(RIGHT_DIR_PIN1, OUTPUT);
  pinMode(RIGHT_DIR_PIN2, OUTPUT);
  pinMode(LEFT_DIR_PIN1, OUTPUT);
  pinMode(LEFT_DIR_PIN2, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
}

void moveForward() {
  digitalWrite(RIGHT_DIR_PIN1, HIGH);
  digitalWrite(RIGHT_DIR_PIN2, LOW);
  digitalWrite(LEFT_DIR_PIN1, HIGH);
  digitalWrite(LEFT_DIR_PIN2, LOW);
}

void turnLeft() {
  digitalWrite(RIGHT_DIR_PIN1, HIGH);
  digitalWrite(RIGHT_DIR_PIN2, LOW);
  digitalWrite(LEFT_DIR_PIN1, LOW);
  digitalWrite(LEFT_DIR_PIN2, HIGH);
}

void turnRight() {
  digitalWrite(RIGHT_DIR_PIN1, LOW);
  digitalWrite(RIGHT_DIR_PIN2, HIGH);
  digitalWrite(LEFT_DIR_PIN1, HIGH);
  digitalWrite(LEFT_DIR_PIN2, LOW);
}

void moveBackward() {
  digitalWrite(RIGHT_DIR_PIN1, LOW);
  digitalWrite(RIGHT_DIR_PIN2, HIGH);
  digitalWrite(LEFT_DIR_PIN1, LOW);
  digitalWrite(LEFT_DIR_PIN2, HIGH);
}

void stopMoving() {
  digitalWrite(RIGHT_DIR_PIN1, LOW);
  digitalWrite(RIGHT_DIR_PIN2, LOW);
  digitalWrite(LEFT_DIR_PIN1, LOW);
  digitalWrite(LEFT_DIR_PIN2, LOW);
  setMotorSpeeds(0, 0);
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  analogWrite(LEFT_SPEED_PIN, leftSpeed);
  analogWrite(RIGHT_SPEED_PIN, rightSpeed);
}

void activateBuzzer() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, LOW);
    delay(2);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2);
  }
}

int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.01657;
  return round(distance);
}

String scanSurroundings() {
  // Implement the logic to scan surroundings and return a binary string
  // representing obstacles in different directions
  // This is a placeholder and should be implemented based on your specific requirements
  return "00000";
}

void avoidObstacles() {
  int distance = measureDistance();
  if (distance < FRONT_DISTANCE_LIMIT) {
    stopMoving();
    String obstaclePattern = scanSurroundings();
    
    if (obstaclePattern == "10000") {
      setMotorSpeeds(FAST_SPEED, NORMAL_SPEED);
      moveForward();
      delay(TURN_TIME);
    } else if (obstaclePattern == "00001") {
      setMotorSpeeds(NORMAL_SPEED, FAST_SPEED);
      moveForward();
      delay(TURN_TIME);
    } else if (obstaclePattern == "11100" || obstaclePattern == "01000" || obstaclePattern == "11000") {
      turnRight();
      setMotorSpeeds(TURN_SPEED, TURN_SPEED);
      delay(TURN_TIME);
    } else if (obstaclePattern == "00010" || obstaclePattern == "00111" || obstaclePattern == "00011") {
      turnLeft();
      setMotorSpeeds(TURN_SPEED, TURN_SPEED);
      delay(TURN_TIME);
    } else if (obstaclePattern == "01111" || obstaclePattern == "10111" || obstaclePattern == "11111") {
      turnLeft();
      setMotorSpeeds(FAST_SPEED, NORMAL_SPEED);
      delay(REVERSE_TIME);
    } else if (obstaclePattern == "11011" || obstaclePattern == "11101" || obstaclePattern == "11110") {
      turnRight();
      setMotorSpeeds(NORMAL_SPEED, FAST_SPEED);
      delay(REVERSE_TIME);
    }
    
    stopMoving();
  } else {
    setMotorSpeeds(NORMAL_SPEED, NORMAL_SPEED);
    moveForward();
    delay(REVERSE_TIME);
    stopMoving();
  }
}

void setup() {
  setupMotors();
  stopMoving();
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  
  headServo.attach(SERVO_PIN);
  headServo.write(90);
  
  delay(2000);
  Serial.begin(9600);
}

void loop() {
  avoidObstacles();
}