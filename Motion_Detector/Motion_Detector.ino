int sensorPinX = A0;    // select the input pin for the X axis
//int sensorPinY = A1;    // select the input pin for the Y axis
//int sensorPinZ = A2;    // select the input pin for the Z axis

int sensorValueX;
//int sensorValueY;
//int sensorValueZ;

int xReadings[30];

void setup() {
  Serial.begin(9600);
}

void loop() {
    
  int i;
  for (i = 0; i < 30; i++) {
  delay(1000);
    xReadings[i] = analogRead(sensorPinX);
    Serial.print(i); 
    Serial.print("\t");  
    Serial.println(xReadings[i]);
  }
  
  
}

