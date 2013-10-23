#include <Servo.h>

// Shift Resistor

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
 
byte leds = 0;

// Servo

Servo myservo;  // create servo object to control a servo
int servoPin = 9; // set first servo to PIN 9

// Acceleromator

int sensorPinX = A0; // set X-acceleromator to A0

// Button

const int buttonPin = 2; // pin for button
boolean buttonState; // default button state

// Time

long previousMillis = 0;
long interval = 1000;

int s; // variable to count secs
int h; // variable to count hours
int hourLength = 30; // set to 3600 for full hour

// Motion

int xReadings[30]; // create an array for 30 secs of readings
//int xReadings[3600]; // create an array for one hour of readings

int prevReading= 0; // will get one false reading
int threshold = 3; // tolerance for motion

int motionCounter; // motion count (beyond threshold)

void setup() {

  Serial.begin(9600);
  myservo.attach(servoPin);  // attaches the servo on PIN 9 to the servo object
  myservo.write(90);
  
  pinMode(buttonPin, INPUT); // attaches button to PIN 2

  pinMode(latchPin, OUTPUT); // shift resistor
  pinMode(dataPin, OUTPUT);  // shift resistor
  pinMode(clockPin, OUTPUT); // shift resistor
  
  leds = 0;
  bitSet(leds, h);
  updateShiftRegister();

}

void loop() {

  unsigned long currentMillis = millis();
  int m; // used for moving Servo
  buttonState = digitalRead(buttonPin);
  
  if(currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;
    s++;
    Serial.print("Seconds : ");
    Serial.println(s);
    
    motionCounter += hasMoved();
    Serial.print("Motion count: "); 
    Serial.println(motionCounter);
    
    m = map(motionCounter,0,30,150,30);
    Serial.print("Servo position: "); 
    Serial.println(m);
    Serial.println("\n");
    myservo.write(m);
  }
  
  if(s == hourLength) { 
    s = 0;
    h++;
    Serial.print("Hours : ");
    Serial.println(h);
    
    bitSet(leds, h);
    updateShiftRegister();
    
    motionCounter = 0;

    servoPin++;
    myservo.attach(servoPin);
    myservo.write(150);
    Serial.print("Servo : ");
    Serial.println(servoPin-8);
  } 
 
  if (buttonState == 1) {
     Serial.println("Button pressed");

     for (int i = 9; i <= 11; i++) { // this isn't working!
       myservo.attach(i); 
       myservo.write(90);
       }
       
     s = 0;
     h = 0;
     leds = 0;
     updateShiftRegister();
         
     servoPin = 9;
     myservo.attach(servoPin);
     delay(5000);
   
    }
}

int hasMoved() {
  int reading = analogRead(sensorPinX);
  int dif = abs(reading- prevReading);
  prevReading = reading; // set current reading to be previous
  if (dif > threshold) {
    return 1;
    Serial.println("1");
  } 
  else {
    return 0;
    Serial.println("0");
  } 
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
