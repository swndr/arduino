#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

String GSMshield2 = "1234567890"; // whatever other number is!

// Array to hold the number a SMS is retreived from
char senderNumber[20]; 

String msgArray[10]; // for all the messages needed to win (make bigger)

String msg; // each message
int charLength; // message length

String sender; // sender number
String previousSender = 0000000000; // dumby number to compare to first sender

int i = 0;

int LEDs[11] = {
  0,1,2,3,4,5,6,7,8,9,10}; // array for the LEDs
int lights = 0; // to step through LED array
int currentLED = 0; // to indicate current LED

boolean printMsgs = false; // to stop printing until winner wins

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
    sender = String(senderNumber); // store sender number as a string

    // An example of message disposal    
    // Any messages starting with # should be discarded
    if(sms.peek()=='#')
    {
      Serial.println("Discarded SMS");
      sms.flush();
    }
    
    if (sender = GSMshield2) {
      while(c=sms.read()) {
        //  Serial.print(c);
        msg += c; // build message string
      }
      
      Serial.println("Message from other Arduino");
      Serial.println(msg);
      
      if (msg == "RESET") {
        sender = "0000000000";
        lights = 0;
        currentLED = 0;         
      }
      
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
      Serial.println(charLength);

      sms.beginSMS(senderNumber);
      sms.print("Got this: " + msg); // reply to sender
      sms.endSMS();

      delay(200);

      lights += charLength; // add character length of message to lights count
      Serial.println(lights);

      msgArray[i] = msg; // add message to array
      i++;
      msg = ""; // clear message ready for next one

      delay(200);

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");

      previousSender = sender; // set current sender as previous
      Serial.println("Stored number : " + previousSender);

    } 
    else if (sender == previousSender) { // reply if sender has texted twice in a row
      sms.beginSMS(senderNumber);
      sms.print("You can't text twice in a row. Find a buddy to help you!");
      sms.endSMS(); 

      // Delete message from modem memory
      sms.flush();
      Serial.println("MESSAGE DELETED");
    }

  }

  delay(1000);

  if (lights >= 20) { // to change to <= 160 (enough to move to next light)
    currentLED++; // increase LED count
    LEDs[currentLED]; // step through array
    Serial.println("Current LED:");
    Serial.println(LEDs[currentLED]);
    lights = 0; // return lights count to 0 (need to consider 'wasted' characters here?
    printMsgs = true; // for testing, needs moving to if currentLED == 10 (i.e win)
  }

/*  if (currentLED == 10) {
    // something happens as you've won
    printMsgs = true;

    // need to text the other GSM shield to reset!
    sms.beginSMS(GSMshield2);
    sms.print("RESET");
    sms.endSMS(); 

    currentLED = 0;
  } */

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
  }


}

















