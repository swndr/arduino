#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

char GSMshield1[20] = "+16463221076";

// Array to hold the number a SMS is retreived from
char senderNumber[20]; 
char previousSender1[20];
char previousSender2[20];
char previousSender3[20];
char previousSender4[20];

String msgArray[5]; // for all the messages needed to win (make bigger)

String msg; // each message

int i = 0;

int LEDs[6] = {
  0,4,5,6,11,12}; // array for the LEDs
int currentLED = 0; // to indicate current LED

boolean repeatSender = false;

boolean printMsgs = false; // to stop printing until winner wins

boolean reset = false;

boolean processing = false;
boolean received = false;

void setup() 
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  } 

  Serial.println("GSM SHIELD TWO");

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

    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    if (strcmp(senderNumber, previousSender1)  == 0 || strcmp(senderNumber, previousSender2)  == 0 || strcmp(senderNumber, previousSender3)  == 0 || strcmp(senderNumber, previousSender4)  == 0) {
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

    if (msg == "RESET") {

      reset = true;

    }


    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");

    processing = false;
    received = true;

  }

  if (received == true && repeatSender == false && reset == false) {

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

  else if (received == true && repeatSender == true && reset == false) {
    Serial.println("REPEAT SENDER");

    delay(20); 

    sms.beginSMS(senderNumber);
    sms.print("You can't text twice in a row. Find a buddy to help you!");
    sms.endSMS(); 

    delay(20); 

    received = false;
    repeatSender = false;

  }

  if (currentLED == 2 && reset == false) {
    // if (currentLED == 5) {

    printMsgs = true;
    Serial.println("WINNER");
  }


  if (printMsgs == true) {

    Serial.println("SEND TO PRINT LIST"); 

    for (int n=0; n<5; n++) { // stepping through msg array, need to consider size
     // if (msgArray[n] != "") {
        sms.beginSMS(GSMshield1);
        sms.print(msgArray[n]); // reply to sender
        sms.endSMS();
        delay(200);
     // }
    }

    sms.beginSMS(GSMshield1);
    sms.print("PRINT"); // tell GSMshield1 to print
    sms.endSMS();

    delay(200);

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

    printMsgs = false;

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

  }

}

















