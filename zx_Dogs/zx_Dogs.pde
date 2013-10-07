import processing.serial.*;
import cc.arduino.*;

Arduino arduino;
int analogPin1 = 0;
int analogPin2 = 1;
 
float sound; // sound
float slide; // slide

PImage img1;
PImage img2;

void setup(){
 arduino = new Arduino(this, Arduino.list()[12], 9600); 
 
 size(800,800);
 smooth();
 frameRate(60);
 
 img1 = loadImage("puppy.jpg");
 img2 = loadImage("zombie_dog.jpg");
 
}

void draw(){
  
background(255);

sound = arduino.analogRead(analogPin1); // sound input
float soundRead = map(sound,0,1023,0,255); // map to digital values
soundRead = constrain(soundRead,5,50); // constrain range

slide = arduino.analogRead(analogPin2); // slider
float slideRead = map(slide,0,1023,0,255); // map to digital values

if (soundRead > 8) { // only show image above this sound level
float pRange = map(soundRead,5,100,0.4,0.5); // map sound reading for the filter

tint(255,255); // full opacity
image(img1, 0, 0);

tint(255,slideRead); // variable opacity overlaid over img1
image(img2, 0, 0);

filter(THRESHOLD,pRange); // apply threshold filter

}
 
float s = map(sound,0,200,0,1000); // another mapping for sound level

if (keyPressed == true) { // show equalizer style effect if key pressed
for (int i = 0; i < s; i+= 10) {
noFill();
stroke(255,0,0);
strokeWeight(1);
rectMode(CENTER);
rect(width/2,height/2,i,i);
}
}

println(soundRead);

}


