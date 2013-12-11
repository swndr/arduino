#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include <avr/pgmspace.h>

int printer_RX_Pin = 9;  // This is the green wire
int printer_TX_Pin = 10;  // This is the yellow wire

char txtJustify[] = {
  'L', 'C', 'R'};
char txtSize[] = {
  'S', 'M', 'L'};

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);  

void pSetup() {
  printer.begin();
}

void pSeparate() {
  printer.println("-------------------------");
}

void tprint (String printsms, int rJustify, int rStyle) {

printer.setLineHeight(100);

  //  printer.feed(1);

  Serial.println(printsms);
  printer.justify(txtJustify[rJustify]);
  Serial.println(txtJustify[rJustify]);
//  printer.setSize(txtSize[rStyle]);
//  Serial.println(txtSize[rStyle]);
  printer.setSize('L');

 /* if (printsms.length() <= 30) {
    printer.println(printsms);
    // printer.println("\n");
  } 
  else if (printsms.length() > 30 && printsms.length() <= 60) {
    String printsms1 = printsms.substring(0,29);
    String printsms2 = printsms.substring(30,printsms.length());
    printer.println(printsms1);
 //   printer.inverseOn();
    printer.println(printsms2);
 //   printer.inverseOff();
  } 
  else if (printsms.length() > 60 && printsms.length() <= 90) {
    String printsms1 = printsms.substring(0,29);
    String printsms2 = printsms.substring(29,59);
    String printsms3 = printsms.substring(59,printsms.length());
    printer.boldOn();
    printer.println(printsms1);
    printer.boldOff();
    printer.println(printsms2);
    printer.println(printsms3);
  } 
  else if (printsms.length() > 90 && printsms.length() <= 120) {
    String printsms1 = printsms.substring(0,29);
    String printsms2 = printsms.substring(29,59);
    String printsms3 = printsms.substring(59,89);
    String printsms4 = printsms.substring(89,printsms.length());
    printer.println(printsms1);
    printer.println(printsms2);
  //  printer.inverseOn();
    printer.println(printsms3);
  //  printer.inverseOff();
    printer.println(printsms4);
  } 
  else if (printsms.length() > 120) { */
    String printsms1 = printsms.substring(0,29);
    String printsms2 = printsms.substring(29,59);
    String printsms3 = printsms.substring(59,89);
    String printsms4 = printsms.substring(89,119);
    String printsms5 = printsms.substring(119,printsms.length());
    printer.println(printsms1);
   // printer.inverseOn();
    printer.println(printsms2);
   // printer.inverseOff();
    printer.boldOn();
    printer.println(printsms3);
    printer.boldOff();
    printer.println(printsms4);
    printer.println(printsms5);
//  }

  // printer.boldOff();

  printer.feed(1);

}

void printReset () {

  printer.sleep();      // Tell printer to sleep
  printer.wake();       // MUST call wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults

}




