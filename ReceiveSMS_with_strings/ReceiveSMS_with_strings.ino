/*
 SMS receiver
 
 This sketch, for the Arduino GSM shield, waits for a SMS message 
 and displays it through the Serial port. 
 
 Circuit:
 * GSM shield attached to and Arduino
 * SIM card that can receive SMS messages
 
 created 25 Feb 2012
 by Javier Zorzano / TD
 
 This example is in the public domain.
 
 http://arduino.cc/en/Tutorial/GSMExamplesReceiveSMS
 
 */

// include the GSM library
#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];  

String msgArray[5];

String msg;
String sender;

int i = 0;

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
      Serial.print(c);
      msg += c;
    }
    
        // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");

    Serial.println("\nEND OF MESSAGE"); 

    Serial.println("MSG STRING");
    Serial.println(msg);

    if(msg.equals("testing")) {
      Serial.println("MATCH");  
    }

    sms.beginSMS(senderNumber);
    sms.print("Got this: " + msg);
    sms.endSMS(); 
    Serial.println("\nCOMPLETE!\n");

    msgArray[i] = msg;
    Serial.println("Array msg");
    Serial.println(msgArray[i]);
    i++;
    msg = "";

    Serial.println("\nARRAY LIST"); 
    for (int n=0; n<5; n++) {
    Serial.println(msgArray[n]); 
    }



  }

  delay(1000);


}







