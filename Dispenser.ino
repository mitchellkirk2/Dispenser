
#include <AccelStepper.h>
#include <Arduino.h>
#include <ezButton.h>
#include <ArduinoJson.h>
#include <string>
#include <AFMotor.h>


#define motorLStepPin 3
#define motorLDirPin 2
#define motorRStepPin 5 
#define motorRDirPin 4


AccelStepper motorL(AccelStepper::DRIVER, motorLStepPin, motorLDirPin);
AccelStepper motorR(AccelStepper::DRIVER, motorRStepPin, motorRDirPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motorL.setMaxSpeed(500.0);
  motorL.setAcceleration(1000.0);
  motorR.setMaxSpeed(500.0);
  motorR.setAcceleration(1000.0);
  while (Serial.available()) {
    Serial.read(); // Clear the serial buffer
  }
}

void dispenseMixer(int amount){
  int totalTime = amount - 30;

    motorL.move(3500);
    motorR.move(3500);

    while (motorL.distanceToGo() != 0 || motorR.distanceToGo() != 0) {
      motorL.run();
      motorR.run();
    }

    // Pause depending on amount to be dispensed
    Serial.println(amount);
    Serial.println(totalTime);
    for(int i=0;i<=totalTime;i++){
      delay(100);
    }

    // Reverse both motors 
    motorL.move(-3500);
    motorR.move(-3500);

    while (motorL.distanceToGo() != 0 || motorR.distanceToGo() != 0) {
      motorL.run();
      motorR.run();
    }

    // Pause
    delay(300);
  
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.println("DISPENSED"); 
}

void dispenseLiquor(int amount) {
  int totalRotations = amount / 30;

  for (int i = 0; i < totalRotations; i++) {
    // Rotate both motors 
    motorL.move(2150);
    motorR.move(2150);

    while (motorL.distanceToGo() != 0 || motorR.distanceToGo() != 0) {
      motorL.run();
      motorR.run();
    }

    // Pause for 5 seconds
    delay(3000);

    // Reverse both motors 
    motorL.move(-2150);
    motorR.move(-2150);

    while (motorL.distanceToGo() != 0 || motorR.distanceToGo() != 0) {
      motorL.run();
      motorR.run();
    }

    // Pause before repeating
    delay(1500);
  }
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.println("DISPENSED"); 
}

void loop() {

  while (Serial.available()) {
    Serial.read();
  }

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command.startsWith("LIQUOR:")) {
      int amount = command.substring(7).toInt();
      dispenseLiquor(amount);
    } else if (command.startsWith("MIXER:")){
      int amount = command.substring(6).toInt();
      dispenseMixer(amount);
    }
  }
}
