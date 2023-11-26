#include <AccelStepper.h>

#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11

#define inY A0

#define JOYSTICK_UP_THRESHOLD 800
#define JOYSTICK_DOWN_THRESHOLD 100

#define SPEED 200.0

//Stepper motor object. Connected to pins 8, 9, 10, 11.
AccelStepper myStepper(AccelStepper::FULL4WIRE, PIN8, PIN9, PIN10, PIN11);

void setup(){
  //Intialize communication to Serial Monitor
  Serial.begin(9600);

  //Wait for driver to wake up
  delay(10);

  //Set up A0 as y-axis input
  pinMode(inY, INPUT);

  //Where the motor sits is the initial starting position. 
  myStepper.setCurrentPosition(0);

  //Set up stepper acceleration and speed settings. Must show up after setCurrentPostion, since that function sets these two to 0 by default
  myStepper.setAcceleration(1000.0);
  myStepper.setMaxSpeed(1000.0);
}

int yValue;

void loop(){
  //Keep Reading yValue
  yValue = analogRead(inY);

  //Joystick will reliably send an output > 800 if pushed up
  if(yValue > JOYSTICK_UP_THRESHOLD)
  {
    myStepper.setSpeed(SPEED);
  }

  //Joystick will reliably send an output value < 100 if pushed down 
  else if(yValue < JOYSTICK_DOWN_THRESHOLD)
  {
    myStepper.setSpeed(-SPEED);
  } 

  //Otherwise, joystick is somewhere in the middle. Don't move the stepper.
  else 
  {
    myStepper.setSpeed(0);
  }

  //Run the stepper at the end of each loop.
  myStepper.runSpeed();
}
