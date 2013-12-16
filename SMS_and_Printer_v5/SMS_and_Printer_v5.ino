#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

char GSMshield2[20] = "+19292503120";
//char GSMshield2[20] = "+13472416757";

// Array to hold the number a SMS is retreived from
char senderNumber[20]; 

String msgArray[50]; // for all the messages needed to win (make bigger)
String GSM2Array[50]; // to receive from other Arduino

String msg; // each message
int charLength; // message length

String sendToGSMshield;
String sender; // sender number
String previousSender;

boolean processing = false;

int x;

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

  sendToGSMshield = String(GSMshield2);

  previousSender = "+10000000000"; // dummy number to compare to first sender

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
    Serial.println(previousSender);

    if(sms.peek()=='#')
    {
      // reset = true;
      Serial.println("RESETTING");
      sms.flush();
    }

    if (sender == sendToGSMshield) {
      x = 0;
    }
    if (sender == previousSender) {
      x = 1;
    }
    else {
      x = 2;
    }

    switch (x) {
      
    case 0:   
      Serial.println("ARDUINO");

      break;
      
    case 1:   
      Serial.println("PREVIOUS SENDER");

      Serial.println("Second message from : " + previousSender);

      sms.beginSMS(senderNumber);
      sms.print("NOPE"); // reply to sender
      sms.endSMS();

      break;
      
    case 2:  
      Serial.println("NEW SENDER");

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

      String charReport = String(charLength);

      sms.beginSMS(senderNumber);
      sms.print("Got it. You\'re " + charReport + " characters nearer to winning."); // reply to sender
      sms.endSMS();

      charLength = 0;
      msg = "";

      break;

    } 

    previousSender = sender; // set current sender as previous
    Serial.println("Stored number : " + previousSender);

    sms.flush();
  }

  processing = false;

}






















