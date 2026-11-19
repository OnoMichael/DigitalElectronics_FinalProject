#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "BetterButton.h"

// GUItool: begin automatically generated code
AudioSynthWaveform waveform1;  //xy=109,320
AudioOutputI2S i2s1;           //xy=303,317
AudioConnection patchCord1(waveform1, 0, i2s1, 0);
AudioConnection patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=211,375
// GUItool: end automatically generated code




int toggleSwitch = 33;

int volumePotPin = 26;    //A12
int freqPotPin = 27;      // A13
int waveformPotPin = 38;  //A14

int octaveSwitch = 25;

int octaveAdd = 1;

/* 
Final project idea: the ondes martenot ribbon snaps to 1023 different frequencies instead of 12
262 * pow(2, (freqPot) / 1023.0); the formula in Lab 5, but replacing 12 with 1023

Also, make a keyboard above the softpot ribbon that when any of its buttons are pressed,
the softpot ribbon becomes just a pitch bend with more limited range than when the buttons 
are not pressed
*/


/*
can't use pins 7, 8, 20, 21, 23, 18, 19 on the teensy audio adapter 
*/

int waveforms[4] = { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH };

int cButtonPin = 22;
int cSharpButtonPin = 17;
int dButtonPin = 16;
int dSharpButtonPin = 15;
int eButtonPin = 14;
int fButtonPin = 13;
int fSharpButtonPin = 41;
int gButtonPin = 40;
int gSharpButtonPin = 39;
int aButtonPin = 37;
int aSharpButtonPin = 36;
int bButtonPin = 35;
int cHighButtonPin = 34;


int cNote = 262;
int cSharpNote = 277;
int dNote = 294;
int eNote = 330;
int fNote = 349;
int fSharpNote = 370;
int gNote = 392;
int gSharpNote = 415;
int aNote = 440;
int aSharpNote = 466;
int bNote = 494;
int cHighNote = 523;

int keyboardFrequencies[13] = { cNote, cSharpNote, dNote, eNote, fNote, fSharpNote, gNote, gSharpNote, aNote, aSharpNote, bNote, cHighNote };


int keyboardButtonPins[13] = { cButtonPin, cSharpButtonPin, dButtonPin, dSharpButtonPin, eButtonPin, fButtonPin, fSharpButtonPin, gButtonPin, gSharpButtonPin, aButtonPin, aSharpButtonPin, bButtonPin, cHighButtonPin };


BetterButton cButton(cButtonPin, 0);
BetterButton cSharpButton(cSharpButtonPin, 1);
BetterButton dButton(dButtonPin, 2);
BetterButton dSharpButton(dSharpButtonPin, 3);
BetterButton eButton(eButtonPin, 4);
BetterButton fButton(fButtonPin, 5);
BetterButton fSharpButton(fSharpButtonPin, 6);
BetterButton gButton(gButtonPin, 7);
BetterButton gSharpButton(gSharpButtonPin, 8);
BetterButton aButton(aButtonPin, 9);
BetterButton aSharpButton(aSharpButtonPin, 10);
BetterButton bButton(bButtonPin, 11);
BetterButton cHighButton(cHighButtonPin, 12);

BetterButton* buttonArray[13] = { &cButton, &cSharpButton, &dButton, &dSharpButton, &eButton, &fButton, &fSharpButton, &gButton, &gSharpButton, &aButton, &aSharpButton, &bButton, &cHighButton };


void setup() {
  Serial.begin(9600);
  AudioMemory(12);  //always include this when using the Teensy Audio Library even if you don't know what it is yet!
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.0);  //amplitude (volume) can be 0 to 1 and this is usually a good volume for headphones
  waveform1.frequency(262);

  pinMode(toggleSwitch, INPUT);

  pinMode(freqPotPin, INPUT);
  pinMode(volumePotPin, INPUT);
  pinMode(waveformPotPin, INPUT);

  pinMode(octaveSwitch, INPUT);


  for (int i = 0; i < 13; i++) {
    pinMode(keyboardButtonPins[i], INPUT);
    buttonArray[i]->pressHandler(onPress);
    buttonArray[i]->releaseHandler(onRelease);
  }
}

int freqPot = (octaveAdd * 262 * pow(2, (analogRead(A13)) / 1023.0));
int waveformPot = analogRead(A14);
int amplitudePot = analogRead(A12);

void loop() {
  for (int i = 0; i < 13; i++) {
    buttonArray[i]->process();
  }
  waveform1.amplitude(0.5);


  //return;
  freqPot = (octaveAdd * 262 * pow(2, (analogRead(A13)) / 1023.0));
  waveformPot = analogRead(A14);
  amplitudePot = analogRead(A12);







  if (digitalRead(octaveSwitch) == HIGH) {
    octaveAdd = 1;
  } else {
    octaveAdd = 2;
  }

  if (digitalRead(toggleSwitch) == HIGH) {
    waveform1.amplitude(map((float)amplitudePot, 0.0, 1023.0, 0.0, 0.05));  // sets the volume
  } else {
    waveform1.amplitude(0.0);
  }
  waveform1.begin(waveforms[map(waveformPot, 0, 1023, 0, 3)]);  // sets the waveform
  waveform1.frequency(freqPot);                                 //sets the frequency

  //Serial.println(amplitudePot);
}

void onPress(int val) {
  Serial.println("on");
  waveform1.begin(waveforms[map(waveformPot, 0, 1023, 0, 3)]);
  waveform1.frequency(keyboardFrequencies[val]);
}

void onRelease(int val) {
  Serial.println("off");
}