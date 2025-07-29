/*
 * display.h - Display Functions
 * 
 * This file contains all display-related functions for the OLED screen,
 * including mode-specific displays and animations.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

static const unsigned char PROGMEM image_calculator_bits[] = {0x7f,0xe0,0x80,0x10,0xbf,0xd0,0xa0,0x50,0xbf,0xd0,0x80,0x10,0xb6,0xd0,0xb6,0xd0,0x80,0x10,0xb6,0xd0,0xb6,0xd0,0x80,0x10,0xb6,0xd0,0xb6,0xd0,0x80,0x10,0x7f,0xe0};

static const unsigned char PROGMEM image_music_sound_wave_bits[] = {0x00,0x00,0x00,0x02,0x00,0x00,0x02,0x20,0x00,0x02,0x20,0x00,0x02,0x20,0x00,0x0a,0x28,0x00,0x0a,0xa8,0x00,0x2a,0xa8,0x00,0xaa,0xaa,0x80,0x0a,0xaa,0x00,0x0a,0xa8,0x00,0x0a,0x20,0x00,0x02,0x20,0x00,0x02,0x20,0x00,0x02,0x00,0x00,0x00,0x00,0x00};

static const unsigned char PROGMEM image_music_bits[] = {0x00,0x7c,0x0f,0x84,0x08,0x04,0x08,0x7c,0x0f,0xc4,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x38,0x70,0x44,0x88,0x44,0x88,0x38,0x70,0x00,0x00,0x00};


// Function declarations
void startupDisplay();
void updateDisplay();
void drawAnimatedKeyboard();
void drawAnimatedScale();
void drawAnimatedDrums();
String getScaleNoteName(int buttonIndex);

// External variables needed for display functions
extern ControllerMode currentMode;
extern ScaleType currentScale;
extern bool sharpState;
extern int currentOctave;
extern int semitoneOffset;
extern String currentNote;
extern int animationFrame;

void startupDisplay() { 
  display.clearDisplay();

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(38, 6);
  display.print("Midi Calc");

  display.setCursor(35, 17);
  display.print("Controller");

  display.drawBitmap(14, 8, image_calculator_bits, 12, 16, 1);

  display.drawBitmap(100, 8, image_music_sound_wave_bits, 17, 16, 1);

  display.display();

  delay(2000);

  display.clearDisplay();

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(35, 6);
  display.print("fdtschmitz");

  display.setCursor(53, 17);
  display.print("2025");

  display.display();
};

void updateDisplay() {
  display.clearDisplay();
  
  // Show current mode
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Show mode-specific info
  if (currentMode == MODE_STANDARD) {

    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(2, 2);
    display.print("Keyboard Mode");

    display.setCursor(87, 2);
    display.print("Oct:");
    // Show current octave
    display.setCursor(114, 2);
    display.print(currentOctave);
    
    // Show sharp indicator
    if (sharpState == LOW) {
      display.setCursor(110, 13);
      display.setTextSize(2);
      display.print("#");
    }
  } else if (currentMode == MODE_SCALES) {
    // Show current scale
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setTextSize(1);
    display.setCursor(2, 2);
    display.print("Scale Mode");
    display.setCursor(87, 2);
      
    display.setCursor(2, 13);
    // display.setTextSize(2);
    display.print(scaleNames[currentScale]);
    
    // Show current transposition
    display.print("T:");
    if (semitoneOffset >= 0) {
      display.setTextSize(1);
      display.setCursor(114, 2);
      display.print("+");
    }
    display.print(semitoneOffset);
    
  } else if (currentMode == MODE_DRUMS) {
    display.setCursor(2, 2);
    display.print("Drum Mode");
    
  } else if (currentMode == MODE_CHORD) { // NOVO MODO
    display.setCursor(2, 2);
    display.print("Chord Mode");
    display.setCursor(87, 2);
    display.print("Web CFG");
  }
  
  // Current note or animated display
  if (currentNote != "") {
    // Show current note being played
    display.setTextSize(2);
    display.setCursor(80, 13);
    display.print(currentNote);
    
  } else {
    // TODO
  }
  
  display.display();
}


String getScaleNoteName(int buttonIndex) {
  int interval = scaleIntervals[currentScale][buttonIndex];
  String noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  
  // Apply semitone transposition
  int totalSemitones = (interval + semitoneOffset) % 12;
  if (totalSemitones < 0) totalSemitones += 12;
  
  int octave = currentOctave + ((interval + semitoneOffset) / 12);
  
  return noteNames[totalSemitones] + String(octave);
}

#endif // DISPLAY_H