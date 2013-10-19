#include <Servo.h> 

Servo myservo;  // create servo object to control a servo
int servoPin = 9;

int h;

int sensorPinX = A0; 

int xReadings[30]; // create an array for 30 secs of readings
//int xReadings[3600]; // create an array for one hour of readings

int prevReading= 0; // will get one false reading
int threshold = 3;

int motionCounter;

   	
void setup() {
  
  Serial.begin(9600);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  	
}

void loop() {
  
  int i; // variable for iteration
  int m; // used for moving Servo

  
//  for (i = 0; i < 3600; i++) { // use for one hour
  for (i = 0; i < 30; i++) {
    delay(1000);
    motionCounter += hasMoved();
    Serial.print(i); 
    Serial.print("\t");  
    Serial.println(motionCounter);

    
    m = map(motionCounter,0,30,0,180);
    Serial.println(m);
    
    myservo.write(m);
    
    if (i == 29) {
      h++;
      Serial.println(h);
      
      motionCounter = 0;
      
      servoPin++;
      myservo.attach(servoPin);
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
 		} else {
 		return 0;
                Serial.println("0");
 		} 
 }
