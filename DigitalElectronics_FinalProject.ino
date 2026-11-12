#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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

int waveforms[4] = { WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH };

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
}

void loop() {
  if (digitalRead(octaveSwitch) == HIGH) {
    octaveAdd = 1;
  } else {
    octaveAdd = 2;
  }
  int freqPot = (octaveAdd * 262 * pow(2, (analogRead(A13)) / 1023.0));
  int waveformPot = analogRead(A14);
  int amplitudePot = analogRead(A12);
  ;
  if (digitalRead(toggleSwitch) == HIGH) {
    waveform1.amplitude(map((float)amplitudePot, 0.0, 1023.0, 0.0, 0.05));  // sets the volume
  } else {
    waveform1.amplitude(0.0);
  }
  waveform1.begin(waveforms[map(waveformPot, 0, 1023, 0, 3)]);  // sets the waveform
  waveform1.frequency(freqPot);                                 //sets the frequency

  Serial.println(freqPot);
}
