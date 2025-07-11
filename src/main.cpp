/*
 Arduino Pro Micro MIDI Controller - Multi-Mode Musical Controller
 Main File: midi_controller_main.ino
  
 Files structure:
 - midi_controller_main.ino (this file)
 - config.h (pin definitions and constants)
 - modes.h (mode definitions and enums)
 - display.h (display functions)
 - midi_functions.h (MIDI communication functions)
 - button_handlers.h (button handling functions)
  
 Hardware connections:
 OLED Display (I2C):
 - SDA: Pin 2 (SDA)
 - SCL: Pin 3 (SCL)
  
 Button connections:
 - Buttons 1-7: Pins 4-10 to GND (with internal pullup)
 - Button 8, 9, 10: Pins 16, 14 and 15 to GND (with internal pullup)
 - Button 11 (Mode): Pin A0 to GND (with internal pullup)
 
 Mode Functions:
 - Standard Mode: C-D-E-F-G-A-B with sharp and octave controls
 - Scales Mode: Various scales with semitone transposition
 - Drums Mode: All 10 buttons play different drum sounds
 */

#include <MIDIUSB.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "modes.h"
#include "display.h"
#include "midi_functions.h"
#include "button_handlers.h"

// Global variables
ControllerMode currentMode = MODE_STANDARD;
ScaleType currentScale = SCALE_MAJOR;

// State variables
bool noteStates[numNoteButtons];
bool lastNoteStates[numNoteButtons];
bool sharpState = false;
bool lastSharpState = false;
bool octaveDownState = false;
bool lastOctaveDownState = false;
bool octaveUpState = false;
bool lastOctaveUpState = false;
bool modeState = false;
bool lastModeState = false;

int currentOctave = 4;
int octaveOffset = 0;
int semitoneOffset = 0;

int playedMidiNotes[numNoteButtons + 3]; // +3 for extra drum buttons
bool noteIsPlaying[numNoteButtons + 3];

unsigned long lastDebounceTime[numNoteButtons + 4];

String currentNote = "";
unsigned long displayTimeout = 0;

unsigned long lastAnimationUpdate = 0;
int animationFrame = 0;

void setup() {
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  // Show startup message
  startupDisplay();

  delay(2500);
  
  // Initialize button pins
  for (int i = 0; i < numNoteButtons; i++) {
    pinMode(notePins[i], INPUT_PULLUP);
    noteStates[i] = HIGH;
    lastNoteStates[i] = HIGH;
    lastDebounceTime[i] = 0;
    playedMidiNotes[i] = 0;
    noteIsPlaying[i] = false;
  }
  
  // Initialize extra drum buttons
  for (int i = numNoteButtons; i < numNoteButtons + 3; i++) {
    playedMidiNotes[i] = 0;
    noteIsPlaying[i] = false;
  }
  
  pinMode(sharpPin, INPUT_PULLUP);
  pinMode(octaveDownPin, INPUT_PULLUP);
  pinMode(octaveUpPin, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);
  
  // Initialize states
  sharpState = HIGH;
  lastSharpState = HIGH;
  octaveDownState = HIGH;
  lastOctaveDownState = HIGH;
  octaveUpState = HIGH;
  lastOctaveUpState = HIGH;
  modeState = HIGH;
  lastModeState = HIGH;
  
  // Initialize debounce times
  for (int i = 0; i < numNoteButtons + 4; i++) {
    lastDebounceTime[i] = 0;
  }
  
  // Initialize serial
  Serial.begin(9600);
  
  // Show initial display
  updateDisplay();
  
  delay(100);
}

void loop() {
  // Handle mode button
  handleModeButton();
  
  // Handle other buttons based on current mode
  if (currentMode == MODE_STANDARD) {
    handleSharpButton();
    handleOctaveButtons();
    // Handle 7 note buttons
    for (int i = 0; i < numNoteButtons; i++) {
      handleNoteButton(i);
    }
  } else if (currentMode == MODE_SCALES) {
    handleScaleButton();
    handleTransposeButtons(); // Changed from octave to transpose
    // Handle 7 note buttons
    for (int i = 0; i < numNoteButtons; i++) {
      handleNoteButton(i);
    }
  } else if (currentMode == MODE_DRUMS) {
    // Handle all 10 buttons as drums
    for (int i = 0; i < numNoteButtons; i++) {
      handleNoteButton(i);
    }
    // Handle the 3 extra buttons (sharp, octave down, octave up) as drums
    handleDrumButton(numNoteButtons, sharpPin);     // Button 8
    handleDrumButton(numNoteButtons + 1, octaveDownPin); // Button 9
    handleDrumButton(numNoteButtons + 2, octaveUpPin);   // Button 10
  }
  
  // Handle all buttons based on current mode
  updateButtons();
  
  // Check if display should timeout
  if (displayTimeout > 0 && millis() > displayTimeout) {
    currentNote = "";
    displayTimeout = 0;
  }
  
  // Update animation when idle
  if (currentNote == "" && millis() - lastAnimationUpdate > ANIMATION_DELAY) {
    int maxFrames = (currentMode == MODE_DRUMS) ? 10 : 7;
    animationFrame = (animationFrame + 1) % maxFrames;
    lastAnimationUpdate = millis();
    updateDisplay();
  }
  
  delay(1);
}

// Global variables that need to be accessible from other files
extern ControllerMode currentMode;
extern ScaleType currentScale;
extern bool sharpState;
extern int currentOctave;
extern int octaveOffset;
extern int semitoneOffset;
extern String currentNote;
extern unsigned long displayTimeout;
extern int animationFrame;
extern int playedMidiNotes[];
extern bool noteIsPlaying[];
extern unsigned long lastDebounceTime[];
extern bool noteStates[];
extern bool lastNoteStates[];
extern bool lastSharpState;
extern bool octaveDownState;
extern bool lastOctaveDownState;
extern bool octaveUpState;
extern bool lastOctaveUpState;
extern bool modeState;
extern bool lastModeState;