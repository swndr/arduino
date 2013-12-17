#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include <avr/pgmspace.h>

int printer_RX_Pin = 9;  // This is the green wire
int printer_TX_Pin = 10;  // This is the yellow wire

/*char txtJustify[] = {
  'L', 'C', 'R'};
char txtSize[] = {
  'S', 'M', 'L'};

int SMSstrings[] = {
  0,10,30,39,49,59,79,99,109,119,139,159};*/

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);  

void pSetup() {
  printer.begin();
}

void tprint (String printsms) {

  Serial.println(printsms);

  for (int i = 0; i < 1; i++) { // single for loop so can use 'continue' function

    // message split into substrings that will format properly
    String printsms1 = printsms.substring(0,10);
    String printsms2 = printsms.substring(10,30);
    String printsms3 = printsms.substring(30,39);
    String printsms4 = printsms.substring(39,49);
    String printsms5 = printsms.substring(49,59);
    String printsms6 = printsms.substring(59,79);
    String printsms7 = printsms.substring(79,99);
    String printsms8 = printsms.substring(99,109);
    String printsms9 = printsms.substring(109,119);
    String printsms10 = printsms.substring(119,139);
    String printsms11 = printsms.substring(139,159);

    printer.inverseOn();
    printer.println(printsms1);
    printer.inverseOff();

    if (printsms.length() < 10) { // skip rest of for loop if message < 10 chars
      continue;
    }

    printer.justify('C');
    printer.boldOn();
    printer.println(printsms2);
    printer.boldOff();

    if (printsms.length() < 30) {
      continue;
    }

    printer.justify('R');
    printer.setSize('L');
    printer.println(printsms3);

    if (printsms.length() < 39) {
      continue;
    }

    printer.justify('L');
    printer.setSize('S');
    printer.println(printsms4);

    if (printsms.length() < 49) {
      continue;
    }

    printer.justify('C');
    printer.inverseOn();
    printer.println(printsms5);
    printer.inverseOff();

    if (printsms.length() < 59) {
      continue;
    }

    printer.justify('L');
    printer.println(printsms6);

    if (printsms.length() < 79) {
      continue;
    }

    printer.justify('R');
    printer.println(printsms7);

    if (printsms.length() < 99) {
      continue;
    }

    printer.inverseOn();
    printer.println(printsms8);
    printer.inverseOff();

    if (printsms.length() < 109) {
      continue;
    }

    printer.justify('L');
    printer.setSize('L');
    printer.println(printsms9);

    if (printsms.length() < 119) {
      continue;
    }

    printer.setSize('S');
    printer.println(printsms10);

    if (printsms.length() < 139) {
      continue;
    }

    printer.setSize('S');
    printer.println(printsms11);


    printer.feed(10); // needs feed
    
  /*  printer.printBarcode("YEAH", CODE39); // max 4?
    printer.setBarcodeHeight(100);
    //  printer.feed(1);*/
  }
}

void printReset () {

  printer.sleep();      // Tell printer to sleep
  printer.wake();       // MUST call wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults

}









