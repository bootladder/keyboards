 
#include "Keyboard.h"

const int SWITCH_ON = 0;  //the switches are active low
const int SWITCH_OFF = 1;
const int NUMBER_OF_SWITCHES = 4;

struct switch_descriptor{
  int pin;
  int state;
  int switchOnKeySequence[10];
  int switchOffKeySequence[10];
};



struct switch_descriptor switchDescriptors[NUMBER_OF_SWITCHES] = {


  {pin : 8,  state : 0xFF,
    switchOnKeySequence  : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'H'} ,
    switchOffKeySequence : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'J'} ,
  } ,
  
    {pin : 9,  state : 0xFF,
    switchOnKeySequence  : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'K'} ,
    switchOffKeySequence : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'L'} ,
  } ,
  
  {pin : 10,  state : 0xFF,
    switchOnKeySequence  : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'O'} ,
    switchOffKeySequence : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'P'} ,
  } ,

  {pin : 11,  state : 0xFF,
    switchOnKeySequence  : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'R'} ,
    switchOffKeySequence : {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'T'} ,
  } ,

};







void setup() {
  
  Keyboard.begin();
  
  for(int i=0; i<NUMBER_OF_SWITCHES; i++){
    pinMode(switchDescriptors[i].pin, INPUT_PULLUP);
  }
}

void loop() {
  delay(100);  //cheap debounce

  for(int i=0; i<NUMBER_OF_SWITCHES; i++){
    handle_switch(&switchDescriptors[i]);
  }
}

void handle_switch(switch_descriptor * switchDescriptor){
  int new_switch_state = read_switch_state(switchDescriptor->pin);
  
  if(new_switch_state != switchDescriptor->state){
    switchDescriptor->state = new_switch_state;


    if(switchDescriptor -> state == SWITCH_ON){

      int * ks = switchDescriptor -> switchOnKeySequence;
      press_and_release_key_sequence(ks);

    }
    else{
      int * ks = switchDescriptor -> switchOffKeySequence;
      press_and_release_key_sequence(ks);
    }

    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
}


int read_switch_state(int pin){
  if(digitalRead(pin) == SWITCH_ON)
    return SWITCH_ON;
  else
    return SWITCH_OFF;
}

void press_and_release_key_sequence(int * ks){
      for(int i=0; ks[i] != 0; i++){
        Keyboard.press(ks[i]);
      }

      for(int i=0; ks[i] != 0; i++){
        Keyboard.release(ks[i]);
      }
}
