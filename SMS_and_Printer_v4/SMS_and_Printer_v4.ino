#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

char GSMshield2[20] = "+19292503120";

// Array to hold the number a SMS is retreived from
char senderNumber[20]; 

String msgArray[50]; // for all the messages needed to win (make bigger)
String GSM2Array[50]; // to receive from other Arduino

String msg; // each message
int charLength; // message length

String sender; // sender number
String previousSender;

int i = 0;
int x = 0;

int LEDs[6] = {
  0,4,5,6,11,12}; // array for the LEDs
int lights = 0; // to step through LED array
int currentLED = 0; // to indicate current LED

boolean processing = false;

boolean printMsgs = false; // to stop printing until winner wins
boolean printGSM2Msgs = false;  // to stop printing until other GSM wins
boolean reset = false;

void setup() 
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 

  Serial.println("SMS Messages Receiver");

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

  previousSender = 0000000000; // dummy number to compare to first sender
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
    sender = String(senderNumber); // store sender number as a string
    Serial.println(sender);

    if (sender == GSMshield2) {
      Serial.println("Message from other Arduino");
      while(c=sms.read()) {
        msg += c; // build message string
      }

      Serial.println("MSG STRING");
      Serial.println(msg);

      if (!msg.equals("PRINT") && !msg.equals("RESET")) {

        GSM2Array[x] = msg; // add message to array
        x++;
        msg = ""; // clear message ready for next one

        // Delete message from modem memory
        sms.flush();
        Serial.println("MESSAGE DELETED");

        delay(100);

      } 

      else if (msg.equals("PRINT")) {
        printGSM2Msgs = true;
        msg = ""; // clear message ready for next one

        // Delete message from modem memory
        sms.flush();
        Serial.println("MESSAGE DELETED");

      }

      else if (msg.equals("RESET")) {

        reset = true;

        sms.flush();
        Serial.println("RESETTING");
      }
    } 

    else if (sender == previousSender) { // reply if sender has texted twice in a row

      sms.beginSMS(senderNumber);
      sms.print("You can't text twice in a row. Find a buddy to help you!");
      sms.endSMS(); 

      msg = ""; // clear message ready for next one

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");
    } 

    else if (sender != previousSender) { // check sender hasn't texted twice
      Serial.println("SENDER OK");

      // Read message bytes and print them
      while(c=sms.read()) {
        //   Serial.print(c);
        msg += c; // build message string
        charLength++; // get length of message
      }

      Serial.println("MSG STRING");
      Serial.println(msg);
      Serial.println("Characters:");
      Serial.println(charLength);

      delay(100);

      // RESET

      if (msg.equals("RESET")) {

        reset = true;

        sms.flush();
        Serial.println("RESETTING");
      }

      else {

        String charReport = String(charLength);

        sms.beginSMS(senderNumber);
        sms.print("Got it. You\'re " + charReport + " characters nearer to winning."); // reply to sender
        sms.endSMS();

        lights += charLength; // add character length of message to lights count
        Serial.println("Light count:");
        Serial.println(lights);

        msgArray[i] = msg; // add message to array
        i++;
        msg = ""; // clear message ready for next one
        charLength = 0;

        previousSender = sender; // set current sender as previous
        Serial.println("Stored number : " + previousSender);

        // Delete message from modem memory
        sms.flush();
        Serial.println("MESSAGE DELETED");

        delay(100);

      }
    }

    processing = false;

  }
  /*
   
   if (lights >= 160) { // to change to <= 160 (enough to move to next light)
   currentLED++; // increase LED count
   digitalWrite(LEDs[currentLED], HIGH); // step through array
   Serial.println("Current LED:");
   Serial.println(LEDs[currentLED]);
   lights -= 160; // continue counting from characters beyond 160
   printMsgs = true; // for testing, needs moving to if currentLED == 10 (i.e win)
   }
   
   if (currentLED == 5) {
   // you've won
   printMsgs = true;
   } 
   
   if (printMsgs == true) {
   
   // need to text the other GSM shield to reset
   sms.beginSMS(GSMshield2);
   sms.print("RESET");
   sms.endSMS();
   
   Serial.println("\nARRAY LIST"); 
   
   pSetup();
   // pSeparate();
   for (int n=0; n<50; n++) { // stepping through msg array, need to consider size
   if (msgArray[n] != "") {
   tprint(msgArray[n]);
   }
   }
   printReset();
   printMsgs = false; 
   
   reset = true;
   
   }
   
   if (printGSM2Msgs == true) {
   
   Serial.println("\nARRAY LIST"); 
   
   pSetup();
   // pSeparate();
   for (int n=0; n<50; n++) { // stepping through msg array, need to consider size
   if (GSM2Array[n] != "") { 
   tprint(GSM2Array[n]); // print array of messages from other Arduino
   }
   }
   
   printReset();
   printGSM2Msgs = false; 
   
   GSM2Array[50] = "";
   
   reset = true;
   
   }
   
   */


  if (reset == true) {

    msgArray[50] = "";

    previousSender = "0000000000";
    lights = 0;
    currentLED = 0;
    charLength = 0;

    msg="";

    for (int n=5; n>0; n--) {
      currentLED = n;
      digitalWrite(LEDs[currentLED], LOW);   
    }

    Serial.println("EVERYTHING RESET");

    reset = false;
  }


}



















