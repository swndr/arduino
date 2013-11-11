#include <Servo.h>

// Set Shift Register pins

int latchPin = 2;
int clockPin = 1;
int dataPin = 0;

byte leds = 0;

// Set Servo pin array to cycle through

int noOfServos = 8;
int servoPinArray[] = {4,5,6,7,8,9,10,11};

// Array to correct reset pins to 90 degrees (compensating for Servo inaccuracy)
int offsetArray[] = {83,90,84,81,87,101,99,96};

Servo myservo;  // create servo object to control a servo

// Acceleromator

int sensorPinX = A0; // set X-acceleromator to A0

// Time variables

long previousMillis = 0;
long interval = 1000;

int s; // variable to count secs
int h; // variable to count hours
int hourLength = 30; // should be set to 3600 for full hour

// Motion

int prevReading = 0; // will get one false reading
int threshold = 1; // tolerance for motion

int motionCounter; // motion count (beyond threshold)

void setup() {

  Serial.begin(9600);

  pinMode(latchPin, OUTPUT); // shift register
  pinMode(dataPin, OUTPUT);  // shift register
  pinMode(clockPin, OUTPUT); // shift register

 // reset all dowels to 90 degrees

  for (int i = 0; i < noOfServos; i++) {
    myservo.attach(servoPinArray[i]); 
    myservo.write(offsetArray[i]);
    delay(350);
    myservo.detach();
  }

  myservo.attach(servoPinArray[0]);

// Switch on first LED

  leds = 0;
  bitSet(leds, h);
  updateShiftRegister();

}

void loop() {

  unsigned long currentMillis = millis();
  int m; // used for moving Servo

// Count seconds 
 
  if(currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;
    s++;
    Serial.print("Seconds : ");
    Serial.println(s);
    
// Count movement via hasMoved function

    motionCounter += hasMoved();
    Serial.print("Motion count: "); 
    Serial.println(motionCounter);

// Map to Servo position and move

    m = map(motionCounter,0,30,125,45);
    Serial.print("Servo position: "); 
    Serial.println(m);
    Serial.println("\n");
    myservo.write(m);
  }
  
// When an hour is reached, progress to next Servo and next LED

  if(s == hourLength) { 
    s = 0;
    h++;
    Serial.print("Hours : ");
    Serial.println(h);

    bitSet(leds, h);
    bitClear(leds, h-1);
    updateShiftRegister();

    motionCounter = 0;
    
    myservo.detach();
    myservo.attach(servoPinArray[h]);
    myservo.write(125);
    Serial.println("Next Servo");
  } 

}

// Function for counting movement

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

// Switch on lights

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

