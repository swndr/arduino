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
String previousSender; // dummy number to compare to first sender

int i = 0;
int x = 0;

int LEDs[6] = {
  0,1,2,3,4,5}; // array for the LEDs
int lights = 0; // to step through LED array
int currentLED = 0; // to indicate current LED

boolean printMsgs = false; // to stop printing until winner wins
boolean printGSM2Msgs = false;  // to stop printing until other GSM wins

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
  Serial.println("Stored number : " + previousSender);
}

void loop() 
{
  char c;

  // If there are any SMSs available()  
  if (sms.available())
  {
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    sender = String(senderNumber); // store sender number as a string
    Serial.println(sender);

    if (sender == GSMshield2) {
      while(c=sms.read()) {
        //  Serial.print(c);
        msg += c; // build message string
      }

      Serial.println("Message from other Arduino");
      Serial.println(msg);

      GSM2Array[x] = msg; // add message to array
      x++;
      msg = ""; // clear message ready for next one

      delay(200);

      if (msg == "PRINT") {
        printGSM2Msgs = true;
      }

      if (msg == "RESET") {
        previousSender = "0000000000";
        lights = 0;
        currentLED = 0;         
      }
      
      msg="";

      sms.flush();
      Serial.println("MESSAGE DELETED");

    }

    if (sender != previousSender) { // check sender hasn't texted twice

      // Read message bytes and print them
      while(c=sms.read()) {
        //  Serial.print(c);
        msg += c; // build message string
        charLength++; // get length of message
      }

      // Serial.println("\nEND OF MESSAGE"); 

      Serial.println("MSG STRING");
      Serial.println(msg);
      Serial.println("Characters:");
      Serial.println(charLength);

      delay(200);

      String charReport = String(charLength);
   //   String reply = " characters nearer to winning.";

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
      
      delay(200);

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");

      previousSender = sender; // set current sender as previous
      Serial.println("Stored number : " + previousSender);

    } 
    else if (sender == previousSender) { // reply if sender has texted twice in a row
    
    Serial.println("Already received from : " + previousSender);
    
      sms.beginSMS(senderNumber);
      sms.print("You can't text twice in a row. Find a buddy to help you!");
      sms.endSMS(); 

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");
    }

  }

  delay(1000);

  if (lights >= 160) { // to change to <= 160 (enough to move to next light)
    currentLED++; // increase LED count
    LEDs[currentLED]; // step through array
    Serial.println("Current LED:");
    Serial.println(LEDs[currentLED]);
    lights -= 160; // continue counting from characters beyond 160
    printMsgs = true; // for testing, needs moving to if currentLED == 10 (i.e win)
  }

   // if (currentLED == 5) {
   // something happens as you've won
  // printMsgs = true;
  // } 

  if (printMsgs == true) {

    Serial.println("\nARRAY LIST"); 

    pSetup();
    // pSeparate();
    for (int n=0; n<9; n++) { // stepping through msg array, need to consider size
      if (msgArray[n] != "") {
        tprint(msgArray[n]);
      }
    }
    printReset();
    printMsgs = false; 

    msgArray[50] = "";

    // need to text the other GSM shield to reset!
    //    sms.beginSMS(GSMshield2);
    //    sms.print("RESET");
    //    sms.endSMS(); 

  }

 if (printGSM2Msgs == true) {

    Serial.println("\nARRAY LIST"); 

    pSetup();
    // pSeparate();
    for (int n=0; n<9; n++) { // stepping through msg array, need to consider size
      if (GSM2Array[n] != "") { 
        tprint(GSM2Array[n]); // print array of messages from other Arduino
      }
    }
    printReset();
    printGSM2Msgs = false; 

    GSM2Array[10] = "";

    previousSender = "0000000000";
    lights = 0;
    currentLED = 0;   

  }


}








