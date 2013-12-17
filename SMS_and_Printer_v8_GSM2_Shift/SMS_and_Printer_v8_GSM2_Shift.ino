#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Shift Register

int latchPin = 9;
int clockPin = 12;
int dataPin = 11;

byte leds = 0;

char GSMshield1[20] = "+16463221076";

// Array to hold the number a SMS is retreived from
char senderNumber[20]; 
char previousSender1[20];
char previousSender2[20];
char previousSender3[20];
char previousSender4[20];

String msgArray[5]; // for all the messages needed to win (make bigger)

String msg; // each message

int i = 0;

int LEDs[6] = {
  0,6,5,4,3,2}; // array for the LEDs
int currentLED = 0; // to indicate current LED

boolean fromOtherArduino = false;
boolean repeatSender = false;

boolean printMsgs = false; // to stop printing until winner wins

boolean reset = false;

boolean processing = false;
boolean received = false;

void setup() 
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT); // shift register
  pinMode(dataPin, OUTPUT);  // shift register
  pinMode(clockPin, OUTPUT); // shift register

  for (int l = 0; l <= 5; l++) {
    bitClear(leds, l);  // step through array
    updateShiftRegister();
  }

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 

  Serial.println("GSM SHIELD TWO");

  // connection state
  boolean notConnected = true;

  // Start GSM connection
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");

  delay(50);

}

void loop() 
{
  char c;

  // If there are any SMSs available()  
  if (sms.available())
  { 
    processing = true;
  }

  if (processing == true) {

    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    if (strcmp(senderNumber, GSMshield1)  == 0) {
      fromOtherArduino = true;
      reset = true;
    }

    else if (strcmp(senderNumber, previousSender1)  == 0 || strcmp(senderNumber, previousSender2)  == 0 || strcmp(senderNumber, previousSender3)  == 0 || strcmp(senderNumber, previousSender4)  == 0) {
      repeatSender = true;
    }

    msg = "";

    // Read message bytes and print them
    while(c=sms.read()) {
      //  Serial.print(c);
      msg.concat(c); // build message string
      delay(5);
    }

    delay(100);

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");

    processing = false;
    received = true;

  }


  if (received == true && fromOtherArduino == false && repeatSender == false && reset == false) {

    Serial.println("MSG STRING");
    Serial.println(msg);

    delay(20);

    sms.beginSMS(senderNumber);
    sms.print("Got it. Get the next person to text."); // reply to sender
    sms.endSMS();

    delay(20); 

    currentLED++; // increase LED count
    bitSet(leds, LEDs[currentLED]);  // step through array
    updateShiftRegister();
    Serial.println("Current LED:");
    Serial.println(currentLED);

    msgArray[i] = msg; // add message to array

    if (i == 0) {
      strncpy(previousSender1, senderNumber, 20); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(previousSender1);
    }

    else if (i == 1) {
      strncpy(previousSender2, senderNumber, 20); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(previousSender2);
    }

    else if (i == 2) {
      strncpy(previousSender3, senderNumber, 20); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(previousSender3);
    }

    else if (i == 3) {
      strncpy(previousSender4, senderNumber, 20); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(previousSender4);
    }

    i++;

    received = false;

  }

  else if (received == true && repeatSender == true && fromOtherArduino == false && reset == false) {
    Serial.println("REPEAT SENDER");

    delay(20); 

    sms.beginSMS(senderNumber);
    sms.print("You can't text twice in a row. Find a buddy to help you!");
    sms.endSMS(); 

    delay(20); 

    received = false;
    repeatSender = false;

  }

  if (currentLED == 2 && reset == false) {
    // if (currentLED == 5) {

    printMsgs = true;
    Serial.println("WINNER");
  }


  if (printMsgs == true) {

    Serial.println("SEND TO PRINT LIST"); 

    for (int n=0; n<1; n++) { // stepping through msg array, need to consider size // BACK TO 5
      sms.beginSMS(GSMshield1);
      sms.print(msgArray[n]); // reply to sender
      sms.endSMS();
      delay(200);
    }

    sms.beginSMS(GSMshield1);
    sms.print("PRINT"); // tell GSMshield1 to print
    sms.endSMS();

    delay(200);

    reset = true;

  }

  if (reset == true) {

    i = 0;

    memset(senderNumber,'\0',20);
    memset(previousSender1,'\0',20);
    memset(previousSender2,'\0',20);
    memset(previousSender3,'\0',20);
    memset(previousSender4,'\0',20);

    msgArray[5] = ("","","","","");

    for (int l = 0; l <= 5; l++) {
      bitClear(leds, l);  // step through array
      updateShiftRegister();
    }

    currentLED = 0;

    Serial.println("EVERYTHING RESET");
    Serial.println(currentLED);

    received = false;
    fromOtherArduino = false;
    repeatSender = false;
    printMsgs = false; 
    processing = false;
    received = false;
    reset = false;

  }

}

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

















