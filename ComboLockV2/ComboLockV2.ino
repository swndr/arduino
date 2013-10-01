const int analogPin = A0;  // pin that the sensor is attached to
const int yellowPin = 11;     // pin that the yellow LEDs are attached to
const int greenPin = 13;   // pin that the yellow LEDs are attached to

int sensorValue = 0;       // the sensor value
int sensorMin = 1023;      // minimum sensor value
int sensorMax = 0;         // maximum sensor value

int blinkSpeed = 0;   // millisecs delay to blink

boolean yellowOn = false;
boolean greenOn = false;

void setup() {
  // initialize the LED pin as an output:
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  // initialize serial communications:
  Serial.begin(9600);
  
    while (millis() < 5000) {
    sensorValue = analogRead(analogPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
}

void loop() {
  // read the value of the potentiometer:
  int analogValue = analogRead(analogPin);
  
  // read the sensor:
  sensorValue = analogRead(analogPin);

  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);

  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  
  // map anolog reading to digital
  int sensorValue = map(analogValue,0,1023,0,255);

  // set range for yellow to blink
  if (sensorValue <= 90 || sensorValue >= 130) {
    yellowOn = true;
    greenOn = false;
    
       if (sensorValue <= 90) { // blink speed for below range
         int blinkSpeed = map(sensorValue,0,90,1000,10);
         digitalWrite(yellowPin, HIGH);
         delay(blinkSpeed);
         digitalWrite(yellowPin, LOW);
         delay(blinkSpeed);
         digitalWrite(greenPin, LOW);
      }
  
      if (sensorValue >= 130) { // blink speed for above range
      int blinkSpeed = map(sensorValue,130,255,10,1000);
      digitalWrite(yellowPin, HIGH);
      delay(blinkSpeed);
      digitalWrite(yellowPin, LOW);
      delay(blinkSpeed);
      digitalWrite(greenPin, LOW);
      }
}
  
  // display green light if within range
  //    yellowOn = false;
  //    greenOn = true;
  else if (sensorValue >= 91 || sensorValue <= 129) {
    digitalWrite(yellowPin,LOW); 
    digitalWrite(greenPin, HIGH);
  }

  // print the analog value:
  Serial.println(sensorValue);
  delay(100);        // delay in between reads for stability
}

