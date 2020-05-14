//  Array of light switches MIDI-serial controller
// 
//  When switch state changes, send a 3-byte MIDI message over the software serial port
//  Using Note-On and Note-Off with Note Number
//

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

const int SWITCH_ON = 0;  //the switches are active low
const int SWITCH_OFF = 1;

struct switch_descriptor{
  int pin;
  int note_number;
  int state;
};

struct switch_descriptor switchDescriptors[8] = {
  {pin : 5, note_number : 20},
  {pin : 6, note_number : 21},
  {pin : 7, note_number : 22},
  {pin : 8, note_number : 23},
  {pin : 9, note_number : 24},
  {pin : 10, note_number : 25},
  {pin : 11, note_number : 26},
  {pin : 12, note_number : 27},
};

const int MIDI_NOTE_ON = 0x90;
const int MIDI_NOTE_OFF = 0x80;

void setup() {
  for(int i=0; i<8; i++){
    pinMode(switchDescriptors[i].pin, INPUT_PULLUP);
  }

  Serial.begin(9600);
  Serial.write("hello");

  pinMode(LED_BUILTIN, OUTPUT);
  mySerial.begin(9600);
}

void loop() {
  delay(100);

  for(int i=0; i<8; i++){
    handle_switch(&switchDescriptors[i]);
  }
}


int read_switch_state(int pin){
  if(digitalRead(pin) == SWITCH_ON)
    return SWITCH_ON;
  else
    return SWITCH_OFF;
}

void handle_switch(switch_descriptor * switchDescriptor){
  int new_switch_state = read_switch_state(switchDescriptor->pin);
  
  if(new_switch_state != switchDescriptor->state){
    switchDescriptor->state = new_switch_state;
    Serial.write(" Sending a new message\n");

    int midi_message[3];
    midi_message[0] = 'A';
    midi_message[1] = 'A';
    midi_message[2] = 'A';

    int midi_status_byte = 0;
    if(switchDescriptor -> state == SWITCH_ON){
      Serial.write(" ON");
      Serial.write('0' + switchDescriptor-> state);
      midi_status_byte = MIDI_NOTE_ON;
    }
    else{
      midi_status_byte = MIDI_NOTE_OFF;
      Serial.write(" OFF");
      Serial.write('0' + switchDescriptor-> state);
    }
      
    
    mySerial.write("XXX");
    mySerial.write(midi_status_byte);
    mySerial.write(switchDescriptor->note_number);
    mySerial.write('U'); //value byte doesn't matter
    mySerial.write("XXX");

    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
}
