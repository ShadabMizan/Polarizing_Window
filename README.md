#Polarizing Window

During my first semester at SFU in Fall 2023, I wrote some code on the Arduino IDE to control a stepper motor that would be spinning a gear that would rotate a polarizing film.
This would adjust the light intesity that would pass through the window, which would allow you to completely block light or get some variation of intensity in between, which was my group's goal.

#Manual Code
This code uses the AccelStepper class, which you can download from the side bar in Arduino IDE.
This code demonstrates how you could control a stepper motor's direction of rotation with a Joystick. 
Furthermore, there are limits in place to dictate how much the motor is allowed to rotate.

#Automatic Code
This code uses the AccelStepper class and the Adafruit_VEML7700 class, both which can be downloaded from Arduino IDE.
the Adafruit_VEML7700 is a light sensor that was used to convert luminosity readings into a step position the motor should move to. 
Like before, there are also limits to how much the motor is allowed to rotate. 

