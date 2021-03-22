#include "Input.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


extern void pressedButton(Game *);
extern void endSelectingMode();
extern void startSelectingMode();
extern void doNothing();
extern Adafruit_SSD1306 display;

extern Game *currentGame;
extern bool isSelectingMode;

void InputAlpha::checkButton() {
  button1->checkButton();
  button2->checkButton();
  button3->checkButton();
}

void InputAlpha::knobChanged(int knobValue) {

  switch (currentGame->getMode()) {
    case METRONOME:
      Metronome *met = currentGame;
      met->setTempo(knobValue + 40);
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0, 0);            // Start at top-left corner
      display.println(F("Metronome"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setTextSize(2);
      //      char s[3] = knobValue;
      //"Tempo is: 60";//String(knobValue + 40);
      char *path = new char[3];

      char cstr[16];
      itoa(knobValue+40, cstr, 10);

      display.println(cstr);
      display.display();
//      delay(200);
      break;
    default:
      Serial.print("no behaviour defined");
  }
}

void InputAlpha::processInput() {

  int knobValue = map(analogRead(A0), 0, 1024, 0, choiceRange);
  if (knobValue != currentChoice) {
    currentChoice = knobValue;
    if (isSelectingMode) {
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0, 0);            // Start at top-left corner
      display.println(F("Select Mode"));
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setTextSize(2);
      switch (knobValue) {

        case 0:
          // Draw 'inverse' text
          display.println("Metronome");
          display.display();
          break;
        case 1:

          display.println(F("Pitch ID"));
          display.display();
          break;
        case 2:

          display.println(F("Lamp"));
          display.display();
          break;
      }
    } else {
      knobChanged(knobValue);
    }
  }
  checkButton();
}

void InputAlpha::pressedButton() {
  Serial.print("\npressed the button \n");

  GameMode mode = currentGame->getMode();

  if (mode == NOTE_ID) {
    Serial.print("its a note id");
    NoteID *noteID = ( NoteID *) currentGame;
    noteID->submitNote(6);
  }
  if (mode == METRONOME) {
    Serial.print("toggle met");
    delay(1000);
    Metronome *met = currentGame;
    met->toggleIsPlaying();
  }
}

void InputAlpha::button1Pressed() {
  Serial.print("button 1 pressed.");
  if (isSelectingMode) {
    endSelectingMode();
  } else {
    Serial.print("not in select mode");
    switch (currentGame->getMode()) {
      case METRONOME:
        (( Metronome*)currentGame)->toggleIsPlaying();
        break;
      case NOTE_ID:
        int knobValue = map(analogRead(A0), 0, 1024, 0, choiceRange);
        (( NoteID*)currentGame)->submitNote(knobValue);
        break;
    }
  }
}

void InputAlpha::button2Pressed() {
  Serial.print("button 2 pressed.");
  switch (currentGame->getMode()) {
    case METRONOME:
      //        (( Metronome*)currentGame)->toggleIsPlaying();
      break;
    case NOTE_ID:
      (( NoteID*)currentGame)->replay();
      break;
  }
}

void InputAlpha::button3Pressed() {
  Serial.print("button 3 pressed.");
  if (isSelectingMode) {
    endSelectingMode();
  } else {
    startSelectingMode();
    //    switch (currentGame->getMode()) {
    //      case METRONOME:
    //                (( Metronome*)currentGame)->toggleIsPlaying();
    //        break;
    //      case NOTE_ID:
    //                int knobValue = map(analogRead(A0), 0, 1024, 0, choiceRange);
    //                (( NoteID*)currentGame)->submitNote(knobValue);
    //        break;
    //      case LAMP:
    //      Serial.print("\nshould start select mode");
    //
    //        break;
    //    }
  }
}

//-----------Button
InputAlpha::InputButton::InputButton( int pin) {
  inputPin = pin;
}
void InputAlpha::InputButton::checkButton() {
  int reading = digitalRead(inputPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();

  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH ) {
        //        if (buttonHeld == false) {
        Serial.print("button pressed.");
        (delegate->*callbackPointer)();
        //        }
        //        buttonHeld = false;
      }
    } else if ((millis() - lastDebounceTime) > 3000) {
      if (buttonState == LOW && buttonHeld == false) {
        Serial.print("button held.");
        buttonHeld = true;
        //        (delegate->*buttonHeldCallback)();
      }
    }
  }

  lastButtonState = reading;
}
