#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 9, 10, 11);
boolean light = true;

void setup()
{
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(150);
// stepper.step(-20);
}

void loop()
{
  
  int LDRvalue = analogRead(0);
  Serial.println(LDRvalue);
  delay(1000);
  
  if (LDRvalue > 70 && light == false) {
    
  // move a number of steps equal to the change in the
  // sensor reading
  stepper.step(-675);
  
  light = true;
  
  }
  
  if (LDRvalue < 70 && light == true) {
    
  // move a number of steps equal to the change in the
  // sensor reading
  stepper.step(675);
  
  light = false;
  
  }

}
