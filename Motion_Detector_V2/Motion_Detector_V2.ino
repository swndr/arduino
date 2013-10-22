#include <Servo.h> 

Servo myservo;  // create servo object to control a servo
int servoPin = 9; // set first servo to PIN 9

int sensorPinX = A0; // set X-acceleromator to A0

const int buttonPin = 2; // pin for button
int buttonState; // default button state

int h; // variable to count hours

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

}

void loop() {



  int i; // variable for iteration
  int m; // used for moving Servo


  //  for (i = 0; i < 3600; i++) { // use for one hour
  for (i = 0; i < 30; i++) {
    delay(1000);
    motionCounter += hasMoved();
    Serial.print("Seconds: "); 
    Serial.println(i); 
    //    Serial.print("\t");  
    Serial.print("Motion count: "); 
    Serial.println(motionCounter);

    m = map(motionCounter,0,30,150,30);
    Serial.print("Servo position: "); 
    Serial.println(m);
    Serial.println("\n");

    myservo.write(m);

    if (i == 29) {
      h++;
      Serial.print("Hour: "); 
      Serial.println(h);

      motionCounter = 0;

      servoPin++;
      myservo.attach(servoPin);
      myservo.write(150);
    }

    buttonState = digitalRead(buttonPin);
    Serial.print("Button: "); 
    Serial.println(buttonState);

    if (buttonState == HIGH) {

      h = 0;
      servoPin = 9;

      for (int r; r < 1; r++) {
        myservo.attach(servoPin+r); 
        myservo.write(90);
        delay(3000);
      }
    }
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

