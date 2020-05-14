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
const int footSwitchD = 11;

const int expressionPedalPin = A0;

enum expressionPedalStateEnum {
  IDLE = 200, MIDDLE = 500, END = 1000
};

int expressionPedalState = IDLE;

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
  pinMode(footSwitchD, INPUT_PULLUP);

  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {

  // All of the switches are active LOW

  handleFootSwitch(footSwitchA, switchBankA_1, switchBankA_2, switchBankA_3);
  //handleFootSwitch(footSwitchB, switchBankB_1, switchBankB_2, switchBankB_3);
  handleFootSwitch(footSwitchC, switchBankB_1, switchBankB_2, switchBankB_3);
  handleFootSwitch(footSwitchD, switchBankB_1, switchBankB_2, switchBankB_3);

  handleKeySequenceFootSwitch(footSwitchB);

  //Handle Expression Pedal
  int expressionPedalValue = analogRead(expressionPedalPin);
  int newExpressionPedalState = IDLE;

  if(expressionPedalValue < IDLE)
    newExpressionPedalState = IDLE;
  if(expressionPedalValue >= IDLE && expressionPedalValue < MIDDLE)
    newExpressionPedalState = MIDDLE;
  if(expressionPedalValue >= MIDDLE)
    newExpressionPedalState = END;
    

  // IDLE to MIDDLE
  if (expressionPedalState == IDLE){
    if (newExpressionPedalState == MIDDLE){
      expressionPedalState = MIDDLE;

      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_ALT);
      Keyboard.release(KEY_LEFT_CTRL);
      
      Keyboard.press(KEY_LEFT_SHIFT);
    }
  }

  // MIDDLE TO IDLE
  if (expressionPedalState == MIDDLE) {
    if (newExpressionPedalState == IDLE){
      expressionPedalState = IDLE;
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_ALT);
      Keyboard.release(KEY_LEFT_CTRL);
    }
  }

  // MIDDLE TO END
  if (expressionPedalState == MIDDLE) {
    if (newExpressionPedalState == END){
      expressionPedalState = END;
      
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_ALT);
      Keyboard.release(KEY_LEFT_CTRL);
      
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
    }
  }

  // END TO MIDDLE
  if (expressionPedalState == END) {
    if (newExpressionPedalState == MIDDLE){
      expressionPedalState = MIDDLE;
      
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_ALT);
      Keyboard.release(KEY_LEFT_CTRL);
      
      Keyboard.press(KEY_LEFT_SHIFT);
    }
  }


}


void handleFootSwitch(int pin, int b1, int b2, int b3){
  if (isFootSwitchPressed(pin)){
    if (digitalRead(b1) == LOW) {
      Keyboard.press(KEY_LEFT_SHIFT);  
    }
    if (digitalRead(b2) == LOW) {
      Keyboard.press(KEY_LEFT_CTRL);  
    }
    if (digitalRead(b3) == LOW) {
      Keyboard.press(KEY_LEFT_ALT);  
    }

    // Wait for release
    while (isFootSwitchPressed(pin)) {
      ;
    }
    Keyboard.release(KEY_LEFT_SHIFT);
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_LEFT_ALT);
  }

}

void handleKeySequenceFootSwitch(int pin){
  Serial.write("hello");
  if (isFootSwitchPressed(pin)){
    Keyboard.write(KEY_UP_ARROW);
    Keyboard.write(KEY_RETURN);
    delay(50);
        // Wait for release
    while (isFootSwitchPressed(pin)) {
      ;
    }
  }
}



int switch2keypress[3] = {
  
};

bool isFootSwitchPressed(int pin){
  return digitalRead(pin) == LOW;

}
