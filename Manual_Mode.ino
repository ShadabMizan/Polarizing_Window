#include <AccelStepper.h>

//Pins that the stepper motor is connected to
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11

//Stepper motor setup. Connected to pins 8, 9, 10, 11.
AccelStepper myStepper(AccelStepper::FULL4WIRE, PIN8, PIN9, PIN10, PIN11);

//Some motor specs to keep settings consistent and similar
#define SPEED 200.0
#define MAX_SPEED 500.0
#define MAX_ACCELERATION 500.0
#define INITIAL_POSITION 0
#define MAX_POSITION_LIMIT -230 //230 steps counterclockwise

//Joystick Y axis input
#define INPUT_Y A0

//When pushed up, joystick will output values above some inconsistent threshold, up to 1023.
//Similarly, when pushed down, joystick outputs values below 100.
#define JOYSTICK_UP_THRESHOLD 600
#define JOYSTICK_DOWN_THRESHOLD 100

void setup(){
  //Intialize communication to Serial Monitor
  Serial.begin(9600);

  //Wait for driver to wake up
  delay(10);

  //Where the motor sits is the initial starting position. 
  myStepper.setCurrentPosition(INITIAL_POSITION);

  //Setting the max limits of the motor
  myStepper.setAcceleration(MAX_ACCELERATION);
  myStepper.setMaxSpeed(MAX_SPEED);

  //Joystick input pin setup
  pinMode(INPUT_Y, INPUT);
}

void loop(){
  //Keep Reading yValue
  int yValue = analogRead(INPUT_Y);
  // Serial.print(yValue); Serial.print("\n");
  Serial.print("Stepper Position: "); Serial.print(myStepper.currentPosition()); Serial.print("\n");
  //Motor should only move if it is under the max step limit
  if(myStepper.currentPosition() <= INITIAL_POSITION && myStepper.currentPosition() >= MAX_POSITION_LIMIT)
  {
    //Joystick will reliably send an output > up threshold if pushed up
    if(yValue > JOYSTICK_UP_THRESHOLD)
    {
      myStepper.setSpeed(-SPEED); //counterclockwise
    }

    //Joystick will reliably send an output value < 100 if pushed down 
    else if(yValue < JOYSTICK_DOWN_THRESHOLD)
    {
      myStepper.setSpeed(SPEED); //clockwise
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
  else if(myStepper.currentPosition() > INITIAL_POSITION)
  {
    myStepper.setCurrentPosition(INITIAL_POSITION);
  }

  //If motor attempts to go over the maximum boundary, just set it back to that maximum boundary
  else if(myStepper.currentPosition() < MAX_POSITION_LIMIT)
  {
    myStepper.setCurrentPosition(MAX_POSITION_LIMIT);
  }
}