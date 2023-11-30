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
#define MIN_LUX 350
#define MAX_LUX 1500

//Some motor specs to keep settings consistent and similar
#define SPEED 400.0
#define MAX_SPEED 500.0
#define MAX_ACCELERATION 500.0
#define INITIAL_POSITION 0
#define MAX_POSITION_LIMIT -250 //250 steps counterclockwise

//Stepper motor setup. Connected to pins 8, 9, 10, 11.
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
  myStepper.setAcceleration(MAX_ACCELERATION);
  myStepper.setMaxSpeed(MAX_SPEED);
}

void loop() {
  //Read light sensor value
  int lux = veml.readLux(VEML_LUX_AUTO);

  //get value from light sensor and convert it to a step position for the motor to rotate to. 
  int nextPosition = map(lux, MIN_LUX, MAX_LUX, INITIAL_POSITION, MAX_POSITION_LIMIT);

  //map function can return values over the maximum limit. Simply make all values < -250 equal to -250
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
