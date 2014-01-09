#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

char GSMshield2[20] = "+19292503120";

// Array to hold the number a SMS is retreived from
char senderNumber[20];

String msgArray[10]; // for all the messages needed to win

String msg; // build message

int n = 0;
int randomMsg;

boolean printMsgs = false; // to stop printing until winner wins
boolean reset = false;

boolean processing = false;
boolean received = false;

boolean confessionAvailable = false;

void setup() 
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 

  Serial.println("GSM SHIELD");

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

  for (int i = 0; i < 10; i++) {
    msgArray[i] = "null";
  }

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

    Serial.println("Message received");
    sms.remoteNumber(senderNumber, 20);

    msg = "";

    // Read message bytes and print them
    while(c=sms.read()) {
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

  if (received == true) {

    Serial.println("MSG STRING");
    Serial.println(msg);

    delay(20);

    if (msgArray[9] == "null") { // before we've received our first 10 confessions

      sms.beginSMS(senderNumber);
      sms.print("We must confess, we don't have enough confessions yet. We'll begin printing when we do."); // reply to sender
      sms.endSMS();

      delay(20);

      msgArray[n] = msg; // add message to array
      n++;

    } 
    else { // when we have enough confessions available

      sms.beginSMS(senderNumber);
      sms.print("Confession received. Someone else's confession is printing for you now."); // reply to sender
      sms.endSMS();

      delay(20);

      for (int i = 0; i < 10; i++) { // look for a random confession to print

        randomMsg = random(9);

        if (msgArray[randomMsg] != "printed") { // break if find one
          confessionAvailable = true;
          break;
        }

      }

      pSetup();

      if (confessionAvailable == true) { // print a confession
        tprint(msgArray[randomMsg]);
        Serial.println(msgArray[randomMsg]);
        confessionAvailable = false;
      } 
      else { // don't have enough confessions available
        tprint("Oops, we're fresh out of confessions! We'll have more soon.");
        Serial.println("No confessions to print");
      }

      printReset();

      msgArray[randomMsg] = "printed";

      msgArray[n] = msg; // add message to array
      n++;

      if (n == 9) { // to begin cycling through array again
        n = 0;
      }

      received = false;

    }

  }

}













































