int sensorPinX = A0;    // select the input pin for the X axis
//int sensorPinY = A1;    // select the input pin for the Y axis
//int sensorPinZ = A2;    // select the input pin for the Z axis

const int ledPin = 13;


int sensorValueX;
//int sensorValueY;
//int sensorValueZ;

int xReadings[30];

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
}

void loop() {
    
  int i;
  int x;
  int pX;
  
  int count = 0;
  
  for (i = 0; i < 30; i++) {
    delay(1000);
    xReadings[i] = analogRead(sensorPinX);
    Serial.print(i); 
    Serial.print("\t");  
    Serial.println(xReadings[i]);
    
    int x = xReadings[i];
    int pX = xReadings[i-1];

    
    if (x == pX) {
      Serial.println("No Motion");
    } else {
      count++;
      Serial.println("Motion");
      
      Serial.println(count);
      
    }
    
    if (i == 29 && count >= 15) {
    digitalWrite(ledPin, HIGH); 
    } else if (i == 29 && count <= 15) {
    digitalWrite(ledPin, LOW); 
  }
  
  }
  
}

