#include <Servo.h>

// Shift Register

int latchPin = 5;
int clockPin = 6;
int dataPin = 4;

byte leds = 0;

// Servo

int noOfServos = 2;
int servoPinArray[] = {9,10};

Servo myservo;  // create servo object to control a servo

// Acceleromator

int sensorPinX = A0; // set X-acceleromator to A0

// Time

long previousMillis = 0;
long interval = 1000;

int s; // variable to count secs
int h; // variable to count hours
int hourLength = 30; // set to 3600 for full hour

// Motion

int prevReading = 0; // will get one false reading
int threshold = 3; // tolerance for motion

int motionCounter; // motion count (beyond threshold)

void setup() {

  Serial.begin(9600);

  pinMode(latchPin, OUTPUT); // shift resistor
  pinMode(dataPin, OUTPUT);  // shift resistor
  pinMode(clockPin, OUTPUT); // shift resistor

  for (int i = 0; i < noOfServos; i++) { // this isn't working!
    myservo.attach(servoPinArray[i]); 
    myservo.write(90);
  }

  myservo.attach(servoPinArray[0]);

  leds = 0;
  bitSet(leds, h);
  updateShiftRegister();

}

void loop() {

  unsigned long currentMillis = millis();
  int m; // used for moving Servo

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

    myservo.attach(servoPinArray[h]);
//    myservo.write(150);
    Serial.println("Next Servo");
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

