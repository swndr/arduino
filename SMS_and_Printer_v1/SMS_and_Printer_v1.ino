#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20]; 
char msgChars[160];

String msgArray[10];

String msg;
String sender;

int i = 0;

boolean printMsgs = false;

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
    Serial.println(senderNumber);
    sender = String(senderNumber);

    // An example of message disposal    
    // Any messages starting with # should be discarded
    if(sms.peek()=='#')
    {
      Serial.println("Discarded SMS");
      sms.flush();
    }

    // Read message bytes and print them
    while(c=sms.read()) {
      //  Serial.print(c);
      msgChars += c;
      msg += c;
    }
    
    for (int m = 0; m < msgChar.size; m++) {
      Serial.print(msgChars[m]);
    }

    delay(200);

    // Serial.println("\nEND OF MESSAGE"); 

    Serial.println("MSG STRING");
    Serial.println(msg);

    delay(200);

    sms.beginSMS(senderNumber);
    sms.print("Got this: " + msg);
    sms.endSMS(); 
    //   Serial.println("\nCOMPLETE!\n");

    delay(200);

    msgArray[i] = msg;
    i++;
    msg = "";

    delay(200);

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");

    printMsgs = true;

  }

  delay(1000);

  if (printMsgs == true) {

    Serial.println("\nARRAY LIST"); 
    pSetup();
    pSeparate();
    for (int n=0; n<9; n++) {
      if (msgArray[n] != "") {
        int randJustify = random(2);
        int randSize = random(2);
       // Serial.println(msgArray[n]);
        tprint(msgArray[n], randJustify, randSize);
      }
    }
    printReset();
    printMsgs = false; 
  }

}















