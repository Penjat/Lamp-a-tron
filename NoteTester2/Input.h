#ifndef HEADER_INPUT
#define HEADER_INPUT
#include "Game.h"



class InputAlpha {
  public:
    virtual void processInput();
    void button3Pressed();
    void button2Pressed();
    void button1Pressed();
    void Create() {
      Serial.print("creating input.");
      button1 = new InputButton(2);
      button1->delegate = this;
      button1->callbackPointer = &InputAlpha::button1Pressed;
      
      button2 = new InputButton(4);
      button2->delegate = this;
      button2->callbackPointer = &InputAlpha::button2Pressed;

      button3 = new InputButton(3);
      button3->delegate = this;
      button3->callbackPointer = &InputAlpha::button3Pressed;
    }
    void pressedButton();

    int choiceRange = 300;
    int currentChoice;
    
    //-----------Button----------------
    class InputButton {
      public:
        InputButton( int);
        void checkButton();
        InputAlpha *delegate;
        void (InputAlpha::*callbackPointer)();
        void (InputAlpha::*buttonHeldCallback)();
      private:
        int lastButtonState = LOW;
        int buttonState;
        int inputPin;
        unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
        unsigned long debounceDelay = 50;
        bool buttonHeld = false;
    };

  private:

    InputButton *button1;
    InputButton *button2;
    InputButton *button3;

    void checkButton();
    void knobChanged(int);
    
};





#endif
