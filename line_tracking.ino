// Define sensor pins (left to right)
const int SENSORS[5] = {A0, A1, A2, A3, A4};

// Define motor control pins
const int RIGHT_SPEED_PIN = 9;   // RIGHT PWM pin
const int RIGHT_DIR_PIN1 = 12;   // Right Motor direction pin 1
const int RIGHT_DIR_PIN2 = 11;   // Right Motor direction pin 2
const int LEFT_SPEED_PIN = 6;    // LEFT PWM pin
const int LEFT_DIR_PIN1 = 7;     // Left Motor direction pin 1
const int LEFT_DIR_PIN2 = 8;     // Left Motor direction pin 2

// Define speed constants
const int FAST = 150;
const int MEDIUM = 140;
const int SLOW = 130;

// Set up motor pins as outputs
void setupMotors() {
  pinMode(RIGHT_DIR_PIN1, OUTPUT);
  pinMode(RIGHT_DIR_PIN2, OUTPUT);
  pinMode(LEFT_DIR_PIN1, OUTPUT);
  pinMode(LEFT_DIR_PIN2, OUTPUT);
  pinMode(RIGHT_SPEED_PIN, OUTPUT);
  pinMode(LEFT_SPEED_PIN, OUTPUT);
}

// Move the robot forward
void moveForward() {
  digitalWrite(RIGHT_DIR_PIN1, HIGH);
  digitalWrite(RIGHT_DIR_PIN2, LOW);
  digitalWrite(LEFT_DIR_PIN1, HIGH);
  digitalWrite(LEFT_DIR_PIN2, LOW);
  setMotorSpeeds(200, 200);
}

// Turn the robot left
void turnLeft(int duration = 0) {
  digitalWrite(RIGHT_DIR_PIN1, HIGH);
  digitalWrite(RIGHT_DIR_PIN2, LOW);
  digitalWrite(LEFT_DIR_PIN1, LOW);
  digitalWrite(LEFT_DIR_PIN2, HIGH);
  setMotorSpeeds(200, 200);
  delay(duration);
}

// Turn the robot right
void turnRight(int duration = 0) {
  digitalWrite(RIGHT_DIR_PIN1, LOW);
  digitalWrite(RIGHT_DIR_PIN2, HIGH);
  digitalWrite(LEFT_DIR_PIN1, HIGH);
  digitalWrite(LEFT_DIR_PIN2, LOW);
  setMotorSpeeds(200, 200);
  delay(duration);
}

// Move the robot backward
void moveBackward(int duration = 0) {
  digitalWrite(RIGHT_DIR_PIN1, LOW);
  digitalWrite(RIGHT_DIR_PIN2, HIGH);
  digitalWrite(LEFT_DIR_PIN1, LOW);
  digitalWrite(LEFT_DIR_PIN2, HIGH);
  setMotorSpeeds(200, 200);
  delay(duration);
}

// Stop the robot
void stopMoving() {
  digitalWrite(RIGHT_DIR_PIN1, LOW);
  digitalWrite(RIGHT_DIR_PIN2, LOW);
  digitalWrite(LEFT_DIR_PIN1, LOW);
  digitalWrite(LEFT_DIR_PIN2, LOW);
}

// Set the speed of both motors
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  analogWrite(LEFT_SPEED_PIN, leftSpeed);
  analogWrite(RIGHT_SPEED_PIN, rightSpeed);
}

// Read values from all sensors
String readSensors() {
  String sensorValues = "";
  for (int i = 0; i < 5; i++) {
    // Invert readings: 1 for black line, 0 for white surface
    sensorValues += !digitalRead(SENSORS[i]);
  }
  return sensorValues;
}

// Main line following logic
void followLine() {
  String sensorData = readSensors();
  Serial.println(sensorData);

  if (sensorData == "10000") {
    // Line is far left, turn left
    turnLeft();
    setMotorSpeeds(FAST, FAST);
  } else if (sensorData == "10100" || sensorData == "01000" || sensorData == "01100" || 
             sensorData == "11100" || sensorData == "10010" || sensorData == "11010") {
    // Line is slightly left, adjust course
    moveForward();
    setMotorSpeeds(0, FAST);
  } else if (sensorData == "00001") {
    // Line is far right, turn right
    turnRight();
    setMotorSpeeds(FAST, FAST);
  } else if (sensorData == "00011" || sensorData == "00010" || sensorData == "00101" || 
             sensorData == "00110" || sensorData == "00111" || sensorData == "01101" || 
             sensorData == "01111" || sensorData == "01011" || sensorData == "01001") {
    // Line is slightly right, adjust cours
    moveForward();
    setMotorSpeeds(FAST, 0);
  } else if (sensorData == "11111") {
    // All sensors detect line, stop moving
    stopMoving();
    setMotorSpeeds(0, 0);
  }
}

// Initial setup
void setup() {
  setupMotors();
  stopMoving();
  Serial.begin(9600);  // Initialize serial communication for debugging
}

// Main program loop
void loop() {
  followLine();
}