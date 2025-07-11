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
  display.setCursor(0, 0);
  display.print("Mode: ");
  display.print(modeNames[currentMode]);
  
  // Show mode-specific info
  if (currentMode == MODE_STANDARD) {
    // Show current octave
    display.setCursor(70, 0);
    display.print("Oct:");
    display.print(currentOctave);
    
    // Show sharp indicator
    if (sharpState == LOW) {
      display.setCursor(110, 0);
      display.print("♯");
    }
  } else if (currentMode == MODE_SCALES) {
    // Show current scale
    display.setCursor(0, 8);
    display.print("Scale: ");
    display.print(scaleNames[currentScale]);
    
    // Show current transposition
    display.setCursor(90, 8);
    display.print("T:");
    if (semitoneOffset >= 0) {
      display.print("+");
    }
    display.print(semitoneOffset);
  } else if (currentMode == MODE_DRUMS) {
    display.setCursor(0, 8);
    display.print("Drum Kit (All 10 btns)");
  }
  
  // Current note or animated display
  if (currentNote != "") {
    // Show current note being played
    display.setTextSize(1);
    display.setCursor(0, 16);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Inverted
    display.print(">");
    display.print(currentNote);
    display.print("<");
  } else {
    // Show animated display based on mode
    if (currentMode == MODE_STANDARD) {
      drawAnimatedKeyboard();
    } else if (currentMode == MODE_SCALES) {
      drawAnimatedScale();
    } else if (currentMode == MODE_DRUMS) {
      drawAnimatedDrums();
    }
  }
  
  display.display();
}

void drawAnimatedKeyboard() {
  // Draw piano keys at the bottom
  int keyWidth = 18;
  int keyHeight = 12;
  int startY = 20;
  int startX = 0;
  
  // Draw 7 white keys (C, D, E, F, G, A, B)
  for (int i = 0; i < 7; i++) {
    int x = startX + (i * keyWidth);
    
    // Draw key outline
    display.drawRect(x, startY, keyWidth, keyHeight, SSD1306_WHITE);
    
    // Animate key press
    if (i == animationFrame) {
      // Fill the key to show it's "pressed"
      display.fillRect(x + 1, startY + 1, keyWidth - 2, keyHeight - 2, SSD1306_WHITE);
      
      // Draw note name on pressed key
      display.setTextSize(1);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(x + 6, startY + 4);
      display.print(noteNames[i]);
    } else {
      // Draw note name on unpressed key
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(x + 6, startY + 4);
      display.print(noteNames[i]);
    }
  }
}

void drawAnimatedScale() {
  // Draw scale degree indicators
  int startY = 20;
  int spacing = 18;
  
  for (int i = 0; i < 7; i++) {
    int x = i * spacing;
    
    // Draw circle for scale degree
    if (i == animationFrame) {
      display.fillCircle(x + 9, startY + 6, 6, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.drawCircle(x + 9, startY + 6, 6, SSD1306_WHITE);
      display.setTextColor(SSD1306_WHITE);
    }
    
    // Draw scale degree number
    display.setTextSize(1);
    display.setCursor(x + 6, startY + 2);
    display.print(i + 1);
  }
}

void drawAnimatedDrums() {
  // Draw drum pad indicators for all 10 buttons
  int startY = 20;
  int spacing = 12; // Smaller spacing to fit 10 buttons
  
  for (int i = 0; i < 10; i++) {
    int x = i * spacing + 4;
    
    // Draw square for drum pad
    if (i == animationFrame) {
      display.fillRect(x, startY, 10, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.drawRect(x, startY, 10, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_WHITE);
    }
    
    // Draw drum number
    display.setTextSize(1);
    display.setCursor(x + 2, startY + 2);
    display.print(i + 1);
  }
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