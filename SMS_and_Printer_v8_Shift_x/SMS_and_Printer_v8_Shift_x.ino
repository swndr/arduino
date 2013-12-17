#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Shift Register
const int latchPin = 12;
const int clockPin = 13;
const int dataPin = 11;

byte leds = 0;

char GSM2[13] = "+19292503120";

// Array to hold the number a SMS is retreived from
char sender[13];
char pSndr1[13];
char pSndr2[13];
char pSndr3[13];
char pSndr4[13];

String msgArray[5]; // for all the messages needed to win

String msg; // build message

byte i = 0;

int LEDs[6] = {
  0,6,5,4,3,2}; // array for the LEDs
int LEDon = 0; // to indicate current LED

boolean otherGSM = false;
boolean pSender = false;

boolean printMsgs = false; // to stop printing until winner wins
boolean reset = false;

boolean processing = false;
boolean received = false;

boolean lost = false;

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

  Serial.println("GSM SHIELD ONE");

  // connection state
  boolean notConnected = true;

  // Start GSM connection
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
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

    Serial.println("Message from:");

    // Get remote number
    sms.remoteNumber(sender, 13);
    Serial.println(sender);

    if (strcmp(sender, GSM2)  == 0) {
      otherGSM = true;
      lost = true;
    }

    else if (strcmp(sender, pSndr1)  == 0 || strcmp(sender, pSndr2)  == 0 || strcmp(sender, pSndr3)  == 0 || strcmp(sender, pSndr4)  == 0) {
      pSender = true;
    }
  
    else if (lost == true && otherGSM == false)
    {
      Serial.println("IGNORING MESSAGE");
      sms.flush();
    } 

    msg = "";

    // Read message bytes and print them
    while(c=sms.read()) {
      msg.concat(c); // build message string
      delay(5);
    }

    delay(100);

    // Delete message from modem memory
    sms.flush();
    Serial.println("DELETED");

    processing = false;
    received = true;

  }

  if (received == true && pSender == false && otherGSM == false && lost == false) {

    Serial.println("MSG");
    Serial.println(msg);

    delay(20);

    sms.beginSMS(sender);
    sms.print("Got it. Get the next person to text."); // reply to sender
    sms.endSMS();

    delay(20); 

    LEDon++; // increase LED count
    bitSet(leds, LEDs[LEDon]);  // step through array
    updateShiftRegister();
    Serial.println("LED:");
    Serial.println(LEDon);

    msgArray[i] = msg; // add message to array

    if (i == 0) {
      strncpy(pSndr1, sender, 13); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(pSndr1);
    }

    else if (i == 1) {
      strncpy(pSndr2, sender, 13); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(pSndr2);
    }

    else if (i == 2) {
      strncpy(pSndr3, sender, 13); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(pSndr3);
    }

    else if (i == 3) {
      strncpy(pSndr4, sender, 13); // set current sender as previous
      Serial.println("Stored number");
      Serial.println(pSndr4);
    }

    i++;

    received = false;

  }

  else if (received == true && pSender == true && lost == false) {
    Serial.println("REPEAT SENDER");

    delay(20); 

    sms.beginSMS(sender);
    sms.print("You can't text twice in a row. Find a buddy to help you!");
    sms.endSMS(); 

    delay(20); 

    received = false;
    pSender = false;

  }

  else if (received == true && otherGSM == true && lost == true) {

    Serial.println("ARDUINO");
    Serial.println(msg);

    if (msg != "PRINT") {
      
        pSetup();
        tprint(msg); // print array of messages from other Arduino
        Serial.println(msg);
         printReset();
         delay(50);
         
      } else {
        
         reset = true;
      }
          
    received = false;
    otherGSM = false;

    }

  if (LEDon == 2 && lost == false) {
    // if (LEDon == 5) {

    printMsgs = true;
    Serial.println("WIN");

    // need to text the other GSM shield to reset
    sms.beginSMS(GSM2);
    sms.print("RESET");
    sms.endSMS();
  } 

  if (printMsgs == true) {

    Serial.println("PRINT LIST"); 

    pSetup();
    for (int n=0; n<5; n++) { // stepping through msg array, need to consider size
      if (msgArray[n] != "") {
        tprint(msgArray[n]);
        Serial.println(msgArray[n]);
      }
    }
    printReset();

    printMsgs = false; 
    reset = true;

  }

  if (reset == true) {

    i = 0;

    memset(sender,'\0',13);
    memset(pSndr1,'\0',13);
    memset(pSndr2,'\0',13);
    memset(pSndr3,'\0',13);
    memset(pSndr4,'\0',13);

    msgArray[5] = ("","","","","");

    for (int l = 0; l <= 5; l++) {
      bitClear(leds, l);  // step through array
      updateShiftRegister();
    }

    LEDon = 0;

    Serial.println("EVERYTHING RESET");

    reset = false;
    lost = false;
//    otherGSM = false;
    pSender = false;
//    printMsgs = false;
    processing = false;
//    received = false;
    
  }

}

void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}


