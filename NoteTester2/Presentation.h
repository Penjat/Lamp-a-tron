#ifndef Presentation_h
#define Presentation_h
#include <FastLED.h>
#define LED_PIN     7
#define NUM_LEDS    12

class Presentation
{
  public:
    Presentation();
    void checkLoop();
    void correct();
    void incorrect();
    void noteSelectStart();
    void modeSelectStart();
    void startMetronome();
    void lampStart();
    void moveClick(int pos);
  private:
    enum EffectMode {NOTE_SELECT, INCORRECT, MODE_SELECT, LAMP, METRONOME};
    CRGB leds[NUM_LEDS];
    CRGB noteColors[12] = {CRGB(0, 255, 0), CRGB(80, 255, 0), CRGB(0, 255, 80), CRGB(0, 143, 255), CRGB(0, 0, 255), CRGB(100, 0, 255), CRGB(255, 0, 200), CRGB(255, 20, 50), CRGB(255, 0, 0), CRGB(255, 50, 0), CRGB(255, 100, 0), CRGB(255, 255, 0)};

    int upSpeed = 2;
    int downSpeed = 1;
    int targetLedFlare[12];
    int curLedFlare[12];

    float curPosition;
    unsigned long previousUpdate = 0;
    EffectMode mode;
    float timer = 10000;
    float moveSpeed = 0.0;
    void setLeds(float);

    void modeSelectLoop();
    void incorrectLoop();
    void selectNoteLoop();
    void lampLoop();
};

#endif
