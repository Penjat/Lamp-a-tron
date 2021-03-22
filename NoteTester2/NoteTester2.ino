#include "Game.h"
#include "Input.h"
#include "Presentation.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

InputAlpha inputAlpha;
Game *currentGame;
Presentation presentationManager;

bool isSelectingMode = true;

void setup() {

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  
  Serial.begin(9600);
  Serial.print("Starting up. \n");

  display.clearDisplay();

  display.invertDisplay(true);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Lampatron activated."));
  display.display();

  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Lamp lamp;
  currentGame = &lamp;
  inputAlpha.Create();
  startSelectingMode();
}

void loop() {
  if (isSelectingMode) {

  } else {
    currentGame->runLoop();
  }
  inputAlpha.processInput();
  presentationManager.checkLoop();
}

// Commands
void startSelectingMode() {
  Serial.print("Entering select mode.");
  inputAlpha.choiceRange = 3;
  isSelectingMode = true;
  presentationManager.modeSelectStart();

  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Select Mode"));
  display.display();
}

void endSelectingMode() {
  Serial.print("Mode selected.");
  switch (inputAlpha.currentChoice) {
    case METRONOME:
      Serial.print("switching to metronome.");
      display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Metronome"));
  display.display();
      currentGame = new Metronome(&presentationManager);
      inputAlpha.choiceRange = 300;
      presentationManager.startMetronome();
      break;
    case NOTE_ID:
      Serial.print("switching to note id.");
      display.clearDisplay();
      display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Note ID"));
  display.display();
      currentGame = new NoteID(&presentationManager);
      inputAlpha.choiceRange = 12;
      presentationManager.noteSelectStart();
      break;
    case LAMP:
    display.clearDisplay();
      Serial.print("switching to lamp mode");
      display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Extreme Lamp Mode"));
  display.display();
      currentGame = new Lamp();
      presentationManager.lampStart();
      break;
    default:
      Serial.print("unkown mode");
      break;
  }
  isSelectingMode = false;
}

void doNothing() {
  Serial.print("unkown mode");
}
