#include <Wire.h>                                          //Load "Wire" library
#include <Adafruit_MotorShield.h>                          //Load "MotorShield" library
#include "utility/Adafruit_MS_PWMServoDriver.h"            //Load Servo & Stepper Drivers

int focusled=7;                                            //Designate pin 2 as "Focusing" led (BLUE)
int shutterNegled=6;                                       //Designate pin 3 as "Shutter DOWN" led (RED)
int shutterPosled=5;                                       //Designate pin 4 as "Shutter UP" led (GREEN)
int moveled=4;                                             //Designate pin 5 as "Moving" led (Diffuse RED)
int shutter=3;                                             //Designate pin 6 as Shutter Control
int focus=2;                                               //Designate pin 7 as Focus Control
int x=0;                                                   //Create integer "x," which equals steps away fro home position (above motor)
int stepSize=7;                                            //Number of steps that will be taken in each movement phase
int shutterSpeed=500;                                      //Define an initial shutter speed of 0.5sec

Adafruit_MotorShield AFMS = Adafruit_MotorShield();        //Create the motor shield object with the default I2C address
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);  //Initialize stepper with (x, _) steps per rev on port (_, y)  [m1 & m2 is port 1, m3 & m4 is port 2]


void setup() {
  AFMS.begin(600);                                         //Run the motor at 600Hz

  pinMode(moveled, OUTPUT);                                //Define "Moving" led (Diffuse RED) as OUTPUT
  pinMode(focusled, OUTPUT);                               //Define "Focusing" led (BLUE) as OUTPUT
  pinMode(shutterPosled, OUTPUT);                          //Define "Shutter Up" led (Green) as OUTPUT
  pinMode(shutterNegled, OUTPUT);                          //Define "Shutter Down" led (RED) as OUTPUT
  pinMode(shutter, OUTPUT);                                //Define shutter control as OUTPUT
  pinMode(focus, OUTPUT);                                  //Define focus control as OUTPUT

  //Focus
  digitalWrite(focusled, HIGH);                            //Turn on "Focusing" led (BLUE)
  digitalWrite(focus, HIGH);                               //Starts focusing
  delay(1500);                                             //Allow 1.5 second to focus
  digitalWrite(focus, LOW);                                //Stop focusing
  digitalWrite(focusled, LOW);                             //Turn off "Focusing" led (BLUE)
  delay(1000);                                             //Wait 1 sec
  
}

void loop() {
  myMotor->setSpeed(10);                                   //Define RPM of motor as 10
  
  //Shutter Release
  digitalWrite(shutter, HIGH);                             //Raise the shutter
  digitalWrite(shutterNegled, LOW);                        //Turn off "Shutter Down" led (RED)
  digitalWrite(shutterPosled, HIGH);                       //Turn on "Shutter Up" led (GREEN)
  //Shutter Speed
  delay(shutterSpeed);                                     //Hold shutter "High" long enough to trigger (>350ms for pentax K3)
  //Shutter Return
  digitalWrite(shutterNegled, HIGH);                       //Turn on "Shutter Down" led (RED)
  digitalWrite(shutterPosled, LOW);                        //Turn off "Shutter Up" led (GREEN)
  digitalWrite(shutter, LOW);                              //Lower the shutter
  delay(1500);                                             //Wait 1.5 second
  

                                                           //Motor not strong enough to run uphill on return as double, use micro.
  digitalWrite(moveled, HIGH);                             //Turn on "Moving" led (Diffuse RED)
  x=x+stepSize;                                            //Add "stepSize" value to previous "x" value to determine new "x" location
  myMotor->step(stepSize, BACKWARD, MICROSTEP);            //Move motor 
  digitalWrite(moveled, LOW);                              //Turn off "Moving" led (Diffuse RED)
  //Settle
  delay (5000);                                            //Wait 5 seconds for vibrations to settle
  
  if (x>=3000){                                            //When the sled reaches the end of the track:
    myMotor->setSpeed(120);                                //Redefine RPM of motor to 120
    digitalWrite(shutterPosled, HIGH);                     //Turn on "Shutter Up" led (GREEN)
    digitalWrite(shutterNegled, HIGH);                     //Turn on "Shutter Down" led (RED)
    digitalWrite(moveled, HIGH);                           //Turn on "Moving" led (Diffuse RED)
    digitalWrite(focusled, HIGH);                          //Turn on "Focusing" led (BLUE)
    delay(3000);                                           //Wait 3 seconds
    myMotor->step(3000, FORWARD, MICROSTEP);               //Return to Origin (x=0)
    digitalWrite(shutterPosled, LOW);                      //Turn off "Shutter Up" led (GREEN)
    digitalWrite(shutterNegled, LOW);                      //Turn off "Shutter Down" led (RED)
    digitalWrite(moveled, LOW);                            //Turn off "Moving" led (Diffuse RED)
    digitalWrite(focusled, LOW);                           //Turn off "Focusing" led (BLUE)
    x=x-3000;                                              //Restart "x" at 0
    delay(5000);                                           //Wait 5 seconds
    }

}
