#include <AccelStepper.h>
#include "Adafruit_VEML7700.h"

//Pins that the stepper motor is connected to
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11

//Set up the light sensor
Adafruit_VEML7700 veml = Adafruit_VEML7700(); 

//Light Sensor boundaries. Expressed in luminosity.
#define MIN_LUX 500
#define MAX_LUX 2000

//Joystick Y axis input
#define INPUT_Y A0

//Joystick will read a value of 1023 if an optimal voltage runs across it. 
//However, lower voltages make this reading decrease, so having a safe threshold around 600 is best.
//Similarly, when pushed down, joystick outputs values below 100.
#define JOYSTICK_UP_THRESHOLD 600
#define JOYSTICK_DOWN_THRESHOLD 100

//Some motor specs to keep settings consistent and similar
#define SPEED 200.0
#define MAX_SPEED 500.0
#define ACCELERATION 500.0
#define INITIAL_POSITION 0
#define MAX_POSITION_LIMIT -230 //230 steps counterclockwise

//Stepper motor setup
AccelStepper myStepper(AccelStepper::FULL4WIRE, PIN8, PIN9, PIN10, PIN11);

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
  myStepper.setAcceleration(ACCELERATION);
  myStepper.setMaxSpeed(MAX_SPEED);
}

void loop(){
  //Read light sensor value
  int lux = veml.readLux(VEML_LUX_AUTO);
  Serial.print("lux: "); Serial.print(lux); Serial.print("\n");

  //Manual Code should run when the light sensor is in a normally lit room
  if(lux < MIN_LUX)
  {
    //Start Reading from Joystick Input
    int yValue = analogRead(INPUT_Y);

    //Serial Monitor Outputs for debugging
    // Serial.print("Y: "); Serial.print(yValue); Serial.print("\n");
    // Serial.print("Stepper Position: "); Serial.print(myStepper.currentPosition()); Serial.print("\n");

    //Motor should only move if it is between the range specified
    if(myStepper.currentPosition() <= INITIAL_POSITION && myStepper.currentPosition() >= MAX_POSITION_LIMIT)
    {

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

  //Automatic code, activates when a flashlight is exposed to the light sensor.
  else if(lux > MIN_LUX)
  {
    //get value from light sensor and convert it to a step position for the motor to rotate to. 
    int nextPosition = map(lux, MIN_LUX, MAX_LUX, INITIAL_POSITION, MAX_POSITION_LIMIT);

    //map function can return values over the maximum limit. Simply make all values less than -230 equal to -230
    if(nextPosition < MAX_POSITION_LIMIT){
      nextPosition = MAX_POSITION_LIMIT;
    } 

    //Same idea as above, need to cap the minimum step position at 0.
    else if(nextPosition > INITIAL_POSITION){
      nextPosition = INITIAL_POSITION;
    }

    //Prep the stepper to move to the calculated position
    myStepper.moveTo(nextPosition);

    //run command needs to be active until the stepper has reach its destination step position.
    while(myStepper.distanceToGo() != 0){
      myStepper.run();
    }
  }
}