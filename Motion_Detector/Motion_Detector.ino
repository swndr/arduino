int sensorPinX = A0;    // select the input pin for the X axis
//int sensorPinY = A1;  // select the input pin for the Y axis
//int sensorPinZ = A2;  // select the input pin for the Z axis

const int ledPin = 13; // select output for LED

int xReadings[30]; // create an array for 30 seconds of readings

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
}

void loop() {
    
  int i; // variable for iteration
  int x; // variable for current X axis reading
  int pX; // variable for previous X axis reading
  
  int count = 0; // variable for couting how much motion
  
  // iterate from 0-29 (30 secs) and display X axis reading
  
  for (i = 0; i < 30; i++) {
    delay(1000);
    xReadings[i] = analogRead(sensorPinX);
    Serial.print(i); 
    Serial.print("\t");  
    Serial.println(xReadings[i]);
    
    int x = xReadings[i]; // set x as current reading
    int pX = xReadings[i-1]; // set pX as last reading

    // compare current and last readings
    // if there's been a change print 'Motion' and add to count variable
    
    if (x == pX) {
      Serial.println("No Motion");
    } else {
      count++;
      Serial.println("Motion");
      
      Serial.print("Count");
      Serial.print("\t");
      Serial.println(count);
      
    }
    
    // set LED state once every 30 seconds
    // if count is more than 15 turn it on (there's been significant motion)
    // if count is less than 15 turn it off (not enough motion)
    
    if (i == 29 && count >= 15) {
    digitalWrite(ledPin, HIGH); 
    } else if (i == 29 && count <= 15) {
    digitalWrite(ledPin, LOW); 
  }
  
  }
  
}

