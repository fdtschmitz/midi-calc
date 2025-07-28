/*
 * config.h - Configuration and Constants
 * 
 * This file contains all pin definitions, constants, and configuration
 * settings for the MIDI controller.
 */


#ifndef CONFIG_H
#define CONFIG_H

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Create display object
extern Adafruit_SSD1306 display;

// Button pins
extern const int notePins[7];
extern const int sharpPin;
extern const int octaveDownPin;
extern const int octaveUpPin;
extern const int modePin;

// Constants
extern const int numNoteButtons;

// Standard mode - Base MIDI notes for C4 scale (60 = C4)
extern const int baseNotes[7];
extern const String noteNames[7];

// MIDI settings
extern const int midiChannel;
extern const int velocity;
extern const int drumChannel;

// Timing constants
extern const unsigned long debounceDelay;
extern const unsigned long DISPLAY_TIMEOUT;
extern const unsigned long ANIMATION_DELAY;

// Initialize display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins
const int notePins[] = {16, 7, 4, 14, 8, 5, 15}; // Buttons 1-7
const int sharpPin = 6;        // Button 8 (Sharp/Scale selector)
const int octaveDownPin = 8;   // Button 9 (Octave/Transpose down)
const int octaveUpPin = 10;     // Button 10 (Octave/Transpose up)
const int modePin = A0;         // Button 11 (Mode selector)

const int numNoteButtons = 7;

// Standard mode - Base MIDI notes for C4 scale (60 = C4)
const int baseNotes[] = {60, 62, 64, 65, 67, 69, 71}; // C, D, E, F, G, A, B
const String noteNames[] = {"C", "D", "E", "F", "G", "A", "B"};

// MIDI settings
const int midiChannel = 0;
const int velocity = 100;
const int drumChannel = 9; // Channel 10 (9 in 0-indexed) for drums

// Timing constants
const unsigned long debounceDelay = 50;
const unsigned long DISPLAY_TIMEOUT = 2000;
const unsigned long ANIMATION_DELAY = 500;

#endif // CONFIG_H