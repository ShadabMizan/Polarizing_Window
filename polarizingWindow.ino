#include <AccelStepper.h>

//Pins that the stepper motor is connected to
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11

//Joystick Y axis input
#define inY A0

//When pushed up, joystick will output values between 800-1000. Similarly, when pushed down, joystick outputs values below 100.
#define JOYSTICK_UP_THRESHOLD 800
#define JOYSTICK_DOWN_THRESHOLD 100

//Some motor specs to keep settings consistent and similar
#define SPEED 200.0
#define INITIAL_POSITION 0
#define MAX_POSITION_LIMIT 200

//Stepper motor setup. Connected to pins 8, 9, 10, 11.
AccelStepper myStepper(AccelStepper::FULL4WIRE, PIN8, PIN9, PIN10, PIN11);

void setup(){
  //Intialize communication to Serial Monitor
  Serial.begin(9600);

  //Wait for driver to wake up
  delay(10);

  //Set up A0 as y-axis input
  pinMode(inY, INPUT);

  //Where the motor sits is the initial starting position. 
  myStepper.setCurrentPosition(INITIAL_POSITION);

  //Set up stepper acceleration and speed settings, set to 1000 each. 
  //In practice, this speed won't really be reached ever, but it's necessary to set a max speed, since by default it's really low.
  myStepper.setAcceleration(1000.0);
  myStepper.setMaxSpeed(1000.0);
}

//Y value to store the readings from the Joystick's Y-axis inputs
int yValue;

void loop(){
  //Keep Reading yValue
  yValue = analogRead(inY);

  //Motor should only move if it is under the max step limit
  if(myStepper.currentPosition() >= INITIAL_POSITION && myStepper.currentPosition() <= MAX_POSITION_LIMIT)
  {
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

  //If motor attempts to go under the minimum boundary, just set it back to the minimum boundary 
  else if(myStepper.currentPosition() <= INITIAL_POSITION)
  {
    myStepper.setCurrentPosition(INITIAL_POSITION);
  }

  //If motor attempts to go over the maximum boundary, just set it back to that maximum boundary
  else if(myStepper.currentPosition() >= MAX_POSITION_LIMIT)
  {
    myStepper.setCurrentPosition(MAX_POSITION_LIMIT);
  }
}
