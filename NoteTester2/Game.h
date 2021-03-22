#ifndef HEADER_GAME
#define HEADER_GAME
#include "Arduino.h"
#include "Presentation.h"

enum GameMode {METRONOME, NOTE_ID, LAMP};

class Game {
  public:
    virtual void runLoop() = 0;
    virtual GameMode getMode() = 0;
};

class Lamp: public Game {
    void runLoop() {

    }

    GameMode getMode() {
      return LAMP;
    }
};

class Metronome: public Game {

  public:
    Metronome(Presentation *presentation) {
      presentationManager = presentation;
    }
    Presentation *presentationManager;
    long interval = 1000;
    unsigned long previousMillis = 0;
    bool isPlaying = false;
    int clickPos = 0;

    void runLoop() {
      checkClick();
    }

    GameMode getMode() {
      return METRONOME;
    }

    void toggleIsPlaying() {
      isPlaying = !isPlaying;
    }

    void setTempo(long tempo) {
      interval = 60000 / tempo;
    }
  private:
    void checkClick() {
      if (isPlaying == true) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          makeClick();
        }
      }
    }
    void makeClick() {
      Serial.print("\nclick\n");
      clickPos = (clickPos+1)%12;
      presentationManager->moveClick(clickPos);
    }
};

class NoteID: public Game {
    enum NoteIDMode {ASKING, WAITING, EDIT};
    NoteIDMode curState;
    int playbackSpeed = 1000;
    int numBeats = 2;
    unsigned long lastTick = 0;
  public:
    NoteID(Presentation *presentation) {
      presentationManager = presentation;
    }
    Presentation *presentationManager;
    void runLoop() {
      switch (curState) {
        case ASKING:
          if (millis() - lastTick  > 1000) {
            lastTick = millis();
            Serial.print("asking");
          }
      }
    }
    GameMode getMode() {
      return NOTE_ID;
    }
    //--------Comands---------
    void submitNote(int note) {
      presentationManager->correct();
    }

    void replay() {
      Serial.print("replaying");
    }
  private:
    int noteArray[12];
    int curBeat = 0;

    void startPlaying() {
      getNewNotes();
      curBeat = 0;
      curState = ASKING;
    }
    void getNewNotes() {
      for (int i = 0; i < numBeats; i++) {
        noteArray[i] = 4;
      }
    }
    void playNote(int note) {
      Serial.print("playing the note.");
    }
};


#endif
