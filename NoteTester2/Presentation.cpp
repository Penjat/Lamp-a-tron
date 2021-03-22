#include "Presentation.h"

Presentation::Presentation() {
  pinMode(5, INPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void Presentation::checkLoop() {
  switch (mode) {
    case MODE_SELECT:
      modeSelectLoop();
      break;
    case NOTE_SELECT:
      selectNoteLoop();
      break;
    case LAMP:
      lampLoop();
      break;
    case INCORRECT:
      incorrectLoop();
      break;
     case METRONOME:
      break;
  }
}

void Presentation::startMetronome() {
  mode = METRONOME;
}
void Presentation::modeSelectStart() {
  mode = MODE_SELECT;
}

void Presentation::noteSelectStart() {
  mode = NOTE_SELECT;
}

void Presentation::lampStart() {
  mode = LAMP;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
}

void Presentation::incorrect() {
  mode = INCORRECT;
  timer = 500;
}

void Presentation::correct() {
  for (int i = 0; i < NUM_LEDS; i++) {
    targetLedFlare[i] = 255;
  }
}

void Presentation::moveClick(int pos) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if(i==pos) {
      leds[i] = CRGB::White;
    } else {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
}

void Presentation::lampLoop() {
  int val = analogRead(A0);
  int knobPositionInt = map(val, 0, 1024, 0, 254);
  FastLED.setBrightness(knobPositionInt );
  FastLED.show();
}

void Presentation::modeSelectLoop() {
  int val = analogRead(A0);
  int knobPositionInt = map(val, 0, 1000, 0, 2);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == knobPositionInt) {
      int val = max(5, abs(sin8_C(millis() * 0.1)));
      leds[i] = CRGB( 255, 255, 255).scale8(CRGB( val, val, val));
    } else if (i < 3) {
      leds[i] = CRGB( 255, 255, 255).scale8(CRGB( 15, 15, 15));
    } else {
      leds[i] = CRGB( 255, 255, 255).scale8(CRGB( 2, 2, 2));
    }

  }
  FastLED.show();
}

void Presentation::selectNoteLoop() {
  int val = analogRead(A0);
  int knobPositionInt = map(val, 0, 1000, 0, 12);
  float knobPosition = static_cast<float>(knobPositionInt);
  float accl = 0.00015;
  float maxSpeed = .04;

  if (abs(curPosition - knobPosition) < 0.05 ) {
    curPosition = knobPosition;
    moveSpeed = 0.0;
  } else {
    moveSpeed += accl;
    curPosition += curPosition > knobPosition ? -moveSpeed : moveSpeed;
  }

  setLeds(curPosition);
}

void Presentation::setLeds(float k) {
  FastLED.clear();

  float knobPosition = k < 12.0 ? k : k -12;

  float highLight = ceil(knobPosition);
  float lowLight = highLight - 1;

  int highIndex =  static_cast<int>(highLight)%11;
  int lowIndex =  static_cast<int>(lowLight)%11;

  CRGB mainColor = noteColors[lowIndex].lerp8(noteColors[highIndex], (1 - (highLight - knobPosition)) * 255);

  for (int i = 0; i < NUM_LEDS; i++) {
    float g = static_cast<float>(i);
    float dist = abs(knobPosition - g) * 270;
    int shouldBeFlare =  max(5, (150 - map(dist, 0, 300, 0, 150)));

    int curFlare = curLedFlare[i];
    int targetFlare = targetLedFlare[i];

    if (shouldBeFlare > targetFlare) {
      targetLedFlare[i] = shouldBeFlare;
    }

    if (curFlare < targetFlare) {
      curLedFlare[i] = curFlare + upSpeed;
      leds[i] = mainColor.scale8(CRGB( curFlare + upSpeed, curFlare + upSpeed, curFlare + upSpeed));
    } else {

      if (abs(curFlare - shouldBeFlare) < 6 ) {
        curLedFlare[i] = targetFlare;
        leds[i] = mainColor.scale8(CRGB( shouldBeFlare, shouldBeFlare, shouldBeFlare));
      } else {
        targetLedFlare[i] = shouldBeFlare;
        curLedFlare[i] = curFlare - downSpeed;
        leds[i] = mainColor.scale8(CRGB( curFlare - downSpeed, curFlare - downSpeed, curFlare - downSpeed));
      }
    }
  }
  FastLED.show();
}

void Presentation::incorrectLoop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int curFlare = max(3, curLedFlare[i] - 1);
    curLedFlare[i] = curFlare;
    leds[i] = CRGB(255, 255, 255).scale8(CRGB( curFlare, curFlare, curFlare));
  }
  FastLED.show();
  timer--;
  if (timer <= 0) {
    mode = NOTE_SELECT;
  }
}
