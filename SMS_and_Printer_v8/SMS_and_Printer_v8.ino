#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// char GSMshield2[20] = "+19292503120";
char GSMshield2[20] = "+13106942711";

// Array to hold the number a SMS is retreived from
char senderNumber[20];
char previousSender1[20];
char previousSender2[20];
char previousSender3[20];
char previousSender4[20];

String msgArray[5]; // for all the messages needed to win
String GSM2Array[5]; // to receive from other Arduino

String msg; // build message

int i = 0;
int x = 0;

int LEDs[6] = {
  0,4,5,6,11,12}; // array for the LEDs
int currentLED = 0; // to indicate current LED

boolean fromOtherArduino = false;
boolean repeatSender = false;

boolean printMsgs = false; // to stop printing until winner wins
boolean printGSM2Msgs = false;  // to stop printing until other GSM wins
boolean reset = false;

boolean processing = false;
boolean received = false;

boolean youLose = false;

void setup() 
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

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

    /*  if(sms.peek()=='#')
     {
     // reset = true;
     Serial.println("RESETTING");
     sms.flush();
     } */

    if(youLose == true)
    {
      Serial.println("IGNORING MESSAGE");
      sms.flush();
      processing = false;
    } 
    else {  

      Serial.println("Message received from:");

      // Get remote number
      sms.remoteNumber(senderNumber, 20);
      Serial.println(senderNumber);

      if (strcmp(senderNumber, GSMshield2)  == 0) {
        fromOtherArduino = true;
        youLose = true;
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
  }

  if (received == true && repeatSender == false && fromOtherArduino == false) {

    Serial.println("MSG STRING");
    Serial.println(msg);

    delay(20);

    sms.beginSMS(senderNumber);
    sms.print("Got it. Get the next person to text."); // reply to sender
    sms.endSMS();

    delay(20); 

    currentLED++; // increase LED count
    digitalWrite(LEDs[currentLED], HIGH); // step through array
    Serial.println("Current LED:");
    Serial.println(LEDs[currentLED]);

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

  else if (received == true && repeatSender == true) {
    Serial.println("REPEAT SENDER");

    delay(20); 

    sms.beginSMS(senderNumber);
    sms.print("You can't text twice in a row. Find a buddy to help you!");
    sms.endSMS(); 

    delay(20); 

    received = false;
    repeatSender = false;

  }

  else if (received == true && fromOtherArduino == true) {

    Serial.println("ARDUINO MSG STRING");
    Serial.println(msg);

    if (msg != "PRINT") {

      GSM2Array[x] = msg; // add message to array
      x++;
      Serial.println("Added to array");

      delay(50);

    }

    else if (msg == "PRINT") {

      printGSM2Msgs = true;

      for (int n=0; n<50; n++) { // stepping through msg array, need to consider size
        if (GSM2Array[n] != "" && GSM2Array[n] != "PRINT") { 
          Serial.println(GSM2Array[n]); // print array of messages from other Arduino
        }
      }

    }

    received = false;
    fromOtherArduino = false;

  }

  if (currentLED == 5) {

    printMsgs = true;
    Serial.println("WINNER");

    // need to text the other GSM shield to reset
    sms.beginSMS(GSMshield2);
    sms.print("RESET");
    sms.endSMS();
  }

  if (printMsgs == true) {

    Serial.println("\nARRAY LIST"); 

    pSetup();
    for (int n=0; n<50; n++) { // stepping through msg array, need to consider size
      if (msgArray[n] != "") {
        tprint(msgArray[n]);
        Serial.println(msgArray[n]);
      }
    }
    printReset();

    printMsgs = false; 
    reset = true;

  }

  if (printGSM2Msgs == true) {

    Serial.println("\nARRAY LIST"); 

    pSetup();
    for (int n=0; n<50; n++) { // stepping through msg array, need to consider size
      if (GSM2Array[n] != "") { 
        tprint(GSM2Array[n]); // print array of messages from other Arduino
        Serial.println(GSM2Array[n]);
      }
    }

    printReset();

    printGSM2Msgs = false; 

    GSM2Array[5] = "";

    reset = true;

  }

  if (reset == true) {

    char senderNumber[20];
    char previousSender1[20];
    char previousSender2[20];
    char previousSender3[20];
    char previousSender4[20];

    msgArray[5] = "";

    currentLED = 0;

    for (int n=5; n>0; n--) {
      currentLED = n;
      digitalWrite(LEDs[currentLED], LOW);   
    }

    Serial.println("EVERYTHING RESET");

    reset = false;
    youLose = false;

  }
}




































