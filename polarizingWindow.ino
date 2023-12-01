//Includes for stepper motor and Adafruit VEML7700 Light Sensor
#include <AccelStepper.h>
#include "Adafruit_VEML7700.h"

//Pins that the stepper motor is connected to
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11

//Stepper motor setup. Connected to pins 8, 9, 10, 11.
AccelStepper myStepper(AccelStepper::FULL4WIRE, PIN8, PIN9, PIN10, PIN11);

//Some motor specs to keep settings consistent and similar
#define SPEED 400.0
#define MAX_SPEED 500.0
#define MAX_ACCELERATION 500.0
#define INITIAL_POSITION 0
#define MAX_POSITION_LIMIT -250 //250 steps counterclockwise

//Light Sensor Setup
Adafruit_VEML7700 veml = Adafruit_VEML7700(); 

//Light Sensor boundaries. Expressed in luminosity.
#define MIN_LUX 350
#define MAX_LUX 1500

//Joystick pin Inputs
#define INPUT_Y A0
#define INPUT_B A1

//Joystick value thresholds
#define JOYSTICK_UP_THRESHOLD 1000
#define JOYSTICK_DOWN_THRESHOLD 100

//Global boolean value to toggle manual and automatic
bool manualMode = false;

void setup() {
  //Intialize communication to Serial Monitor
  Serial.begin(9600);

  //Wait for driver to wake up
  while (!Serial) { 
    delay(10); 
  }

  //Check for the sensor's proper connection
  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");

  //Where the motor sits is the initial starting position. 
  myStepper.setCurrentPosition(INITIAL_POSITION);

  //Setting the max limits of the motor
  myStepper.setAcceleration(MAX_ACCELERATION);
  myStepper.setMaxSpeed(MAX_SPEED);

  //Joystick input pin setup
  pinMode(INPUT_Y, INPUT);
  pinMode(INPUT_B, INPUT);
}

void loop() {
  //Read if the joystick has been pressed down. If pressed down, bValue equals 0. Otherwise, some positive non-zero number. 
  bool bValue = analogRead(INPUT_B);

  //Check if Joystick is pressed down
  if(!bValue)
  {
    //have a half second delay so that multiple readings are not taken in the span of one button press
    delay(500);

    //Switch the boolean
    manualMode = !manualMode;
    
    //more delay, to give the feeling of it switching modes
    delay(500);
  }

  //Automatic Light Sensor Code
  if(!manualMode)
  {
    //Read light sensor value
    int lux = veml.readLux(VEML_LUX_AUTO);

    //get value from light sensor and convert it to a step position for the motor to rotate to. 
    int nextPosition = map(lux, MIN_LUX, MAX_LUX, INITIAL_POSITION, MAX_POSITION_LIMIT);

    //map function can return values over the maximum limit. Simply make all values < -250 equal to -250
    if(nextPosition < MAX_POSITION_LIMIT)
    {
      nextPosition = MAX_POSITION_LIMIT;
    } 

    //Same idea as above, need to cap the minimum step position at 0.
    else if(nextPosition > INITIAL_POSITION)
    {
      nextPosition = INITIAL_POSITION;
    }

    //Prep the stepper to move to the calculated position
    myStepper.moveTo(nextPosition);

    //run command needs to be active until the stepper has reach its destination step position.
    while(myStepper.distanceToGo() != 0)
    {
      myStepper.run();
    }
  }

  //Manual Joystick Control Code
  else if(manualMode)
  {
    //Keep Reading yValue
    int yValue = analogRead(INPUT_Y);

    //Motor should only move if it is under the max step limit
    if(myStepper.currentPosition() <= INITIAL_POSITION && myStepper.currentPosition() >= MAX_POSITION_LIMIT)
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
    else if(myStepper.currentPosition() >= INITIAL_POSITION)
    {
      myStepper.setCurrentPosition(INITIAL_POSITION);
    }

    //If motor attempts to go over the maximum boundary, just set it back to that maximum boundary
    else if(myStepper.currentPosition() <= MAX_POSITION_LIMIT)
    {
      myStepper.setCurrentPosition(MAX_POSITION_LIMIT);
    }
  }


}
