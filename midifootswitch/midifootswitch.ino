/*
Keyboard Foot Switch
Steve Anderson
*/

#include "Keyboard.h"

const int switchBankA_1 = 7;
const int switchBankA_2 = 6;
const int switchBankA_3 = 5;
const int switchBankB_1 = 4;
const int switchBankB_2 = 3;
const int switchBankB_3 = 2;

const int footSwitchA = 8;
const int footSwitchB = 9;
const int footSwitchC = 10;

const int expressionPedalPin = A0;

int expressionPedalValue = 0;
int expressionPedal_MIDI_ID = 9;


#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte id, byte value) {
  midiEventPacket_t midiCc = { 0x0B, 0xB0 | channel, id, value};
  MidiUSB.sendMIDI(midiCc);
}

void setup() {
  pinMode(switchBankA_1, INPUT_PULLUP);
  pinMode(switchBankA_2, INPUT_PULLUP);
  pinMode(switchBankA_3, INPUT_PULLUP);
  pinMode(switchBankB_1, INPUT_PULLUP);
  pinMode(switchBankB_2, INPUT_PULLUP);
  pinMode(switchBankB_3, INPUT_PULLUP);

  pinMode(footSwitchA, INPUT_PULLUP);
  pinMode(footSwitchB, INPUT_PULLUP);
  pinMode(footSwitchC, INPUT_PULLUP);

  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {

  // All of the switches are active LOW
  handleFootSwitch(footSwitchA, 1);
  handleFootSwitch(footSwitchB, 2);
  handleFootSwitch(footSwitchC, 3);

  //Handle Expression Pedal
  int newExpressionPedalValue = analogRead(expressionPedalPin) >> 3; //convert 10-bit value to 7-bit value
  
  if(newExpressionPedalValue != expressionPedalValue){
    controlChange(0,expressionPedal_MIDI_ID,newExpressionPedalValue);
    expressionPedalValue = newExpressionPedalValue;
  }
}

void handleFootSwitch(int pin, int midiNote){
  if (digitalRead(pin) == LOW) {
    
    noteOn(0,midiNote,127);
    MidiUSB.flush();
    delay(200);

    // Wait for release
    while (digitalRead(pin) == LOW) ;
    
    noteOff(0,midiNote,127);
    MidiUSB.flush();
    delay(200);
  }
}


bool isFootSwitchPressed(int pin){
  return digitalRead(pin) == LOW;

}
