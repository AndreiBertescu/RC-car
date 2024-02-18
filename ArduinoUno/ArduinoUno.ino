#include <AFMotor.h>
#include <Servo.h> 

AF_DCMotor motorForwardRight(2);
AF_DCMotor motorForwardLeft(3);
AF_DCMotor motorBackRight(1);
AF_DCMotor motorBackLeft(4);
int speed = 0;
int steering = 0;
bool steeringEnabled = 0;  //true when the servo is used

Servo steeringSero;	// create servo object to control a servo
int servoPos = 0;

//////////////////////SETTINGS
#define SPEED_PIN A2             //speed analog pin
#define STEERING_PIN A1          //steering analog pin
#define STEERING_ENABLE_PIN A0   //steering lock analog/digital pin
void setup() {
  Serial.begin(9600);

  pinMode(SPEED_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);
  pinMode(STEERING_ENABLE_PIN, INPUT);

  steeringSero.attach(10);  
}

void loop() {
  retrieveValues();
  handleMotors();

// for(servoPos = 0; servoPos <= 180; servoPos += 1) 
		// steeringSero.write(0);

  // printValues();
  // printValueTimings();
}

void printValues() {
  Serial.print(speed);
  Serial.print(" ");
  Serial.print(steering);
  Serial.print(" ");
  Serial.print(steeringEnabled);
  Serial.println();
  delay(100);
}

void printValueTimings() {
  Serial.print(pulseIn(SPEED_PIN, HIGH, 980000));
  Serial.print(" ");
  Serial.print(pulseIn(STEERING_PIN, HIGH, 980000));
  Serial.print(" ");
  Serial.print(digitalRead(STEERING_ENABLE_PIN));
  Serial.println();
  delay(100);
}

void retrieveValues(){
  speed = map(pulseIn(SPEED_PIN, HIGH, 980000), 20, 975, -255, 255);
  steering = map(pulseIn(STEERING_PIN, HIGH, 980000), 20, 975, -255, 255);
  speed = (abs(speed) < 1 || abs(speed) >= 256) ? 0 : speed / 10 * 10;
  steering = (abs(steering) < 1 || abs(steering) >= 256) ? 0 : steering / 10 * 10;
  steeringEnabled = digitalRead(STEERING_ENABLE_PIN);
}

void handleMotors(){
  motorForwardRight.setSpeed(abs(speed));
  motorForwardLeft.setSpeed(abs(speed));
  motorBackRight.setSpeed(abs(speed));
  motorBackLeft.setSpeed(abs(speed));

  if(abs(speed) < 20){
    motorForwardRight.run(RELEASE);
    motorForwardLeft.run(RELEASE);
    motorBackRight.run(RELEASE);
    motorBackLeft.run(RELEASE);
  }
  else if(speed >= 20){
      motorForwardRight.run(FORWARD);
      motorForwardLeft.run(FORWARD);
      motorBackRight.run(FORWARD);
      motorBackLeft.run(FORWARD);
  }else if(speed <= -20){
      motorForwardRight.run(BACKWARD);
      motorForwardLeft.run(BACKWARD);
      motorBackRight.run(BACKWARD);
      motorBackLeft.run(BACKWARD);
  }

  if(abs(steering) < 10)
    servoPos = 90;
  else
    servoPos = map(steering, -255, 255, 90+60, 60);
	steeringSero.write(servoPos);

  if(!steeringEnabled && abs(steering) > 20 && abs(speed) < 255/2){
    motorForwardRight.setSpeed(abs(steering));
    motorForwardLeft.setSpeed(abs(steering));
    motorBackRight.setSpeed(abs(steering));
    motorBackLeft.setSpeed(abs(steering));

    if (steering >= 20) {
      motorForwardRight.run(BACKWARD);
      motorForwardLeft.run(FORWARD);
      motorBackRight.run(BACKWARD);
      motorBackLeft.run(FORWARD);
    } else if (steering <= -20) {
      motorForwardRight.run(FORWARD);
      motorForwardLeft.run(BACKWARD);
      motorBackRight.run(FORWARD);
      motorBackLeft.run(BACKWARD);
    }
  }
}
