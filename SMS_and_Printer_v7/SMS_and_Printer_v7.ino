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

String msgArray[50]; // for all the messages needed to win (make bigger)
String GSM2Array[50]; // to receive from other Arduino

String msg; // each message
int charLength; // message length
String charReport; // String to reply to sender

//String GSMshieldCheck = "+13106942711";
String GSMshieldCheck;
//String sender; // sender number

int i = 0;
int x = 0;

// char s;

int LEDs[6] = {
  0,4,5,6,11,12}; // array for the LEDs
int lights = 0; // to step through LED array
int currentLED = 0; // to indicate current LED

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

  // GSMshieldCheck = "+13106942711";

}

void loop() 
{
 char c;

  // If there are any SMSs available()  
  if (sms.available())
  {

    /*  if(sms.peek()=='#')
     {
     reset = true;
     Serial.println("RESETTING");
     //  sms.flush();
     } */

    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    //  String sender = String(senderNumber); // store sender number as a string
    Serial.println(senderNumber);

/*    //    if (sender.equals(GSMshieldCheck)) {
    if (strcmp(senderNumber, GSMshield2)  == 0) {

      Serial.println("Message from other Arduino");

      // Read message bytes and print them
      while(c=sms.read()) {
        msg += c; // build message string
        charLength++; // get length of message
      }

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");

      delay(50);

      Serial.println("MSG STRING");
      Serial.println(msg);

      if (msg != "PRINT") {

        GSM2Array[x] = msg; // add message to array
        x++;
        // msg = ""; // clear message ready for next one

        delay(100);

      } 

      else if (msg == "PRINT") {
        printGSM2Msgs = true;
        Serial.println("PRINT NOW");
        // msg = ""; // clear message ready for next one

      } 

      msg = "";

    } 

    else { */

      Serial.println("HUMAN SENDER");

      // Read message bytes and print them
      while(c=sms.read()) {
        msg += c; // build message string
        charLength++; // get length of message
      }

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");

      delay(50); 

      Serial.println("MSG STRING");
      Serial.println(msg);
      Serial.println("Characters:");
      Serial.println(charLength);

      charReport = String(charLength);

      delay(50); 

      sms.beginSMS(senderNumber);
      sms.print("Got it. You\'re " + charReport + " characters nearer to winning."); // reply to sender
      sms.endSMS();

      delay(50); 

      lights += charLength; // add character length of message to lights count
      Serial.println("Light count:");
      Serial.println(lights);

      msgArray[i] = msg; // add message to array
      i++; 

      charLength = 0;
      msg = "";

  //  }

  }

  delay(50);

  if (lights >= 160) { // to change to <= 160 (enough to move to next light)
    currentLED++; // increase LED count
    digitalWrite(LEDs[currentLED], HIGH); // step through array
    Serial.println("Current LED:");
    Serial.println(LEDs[currentLED]);
    lights -= 160; // continue counting from characters beyond 160
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

  if (reset == true) {

    msgArray[50] = "";

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


























