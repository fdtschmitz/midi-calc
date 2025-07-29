/*
 * button_handlers.h - Button Handling Functions
 * 
 * This file contains all button handling functions including debouncing,
 * mode switching, and note triggering for different modes.
 */

#ifndef BUTTON_HANDLERS_H
#define BUTTON_HANDLERS_H

// External variable declarations (defined in main file)
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

// Function declarations
void updateButtons();
void handleNoteButton(int index);
void handleDrumButton(int index, int pin);
void handleOctaveButtons();
void handleTransposeButtons();
void handleModeButton();
void handleSharpButton();
void handleScaleButton();
// Note: stopAllPlayingNotes() is defined in midi_functions.h

// === NOTE BUTTON HANDLER ===
void handleNoteButton(int index) {
  int reading = digitalRead(notePins[index]);

  if (reading != lastNoteStates[index]) {
    lastDebounceTime[index] = millis();
  }

  if ((millis() - lastDebounceTime[index]) > debounceDelay) {
    if (reading != noteStates[index]) {
      noteStates[index] = reading;

      if (noteStates[index] == LOW) { // Pressed
        int note;
        if (currentMode == MODE_STANDARD) {
          note = calculateStandardMidiNote(index);
        } else if (currentMode == MODE_SCALES) {
          note = calculateScaleMidiNote(index);
        } else { // MODE_DRUMS
          note = calculateDrumMidiNote(index);
        }
        
        playedMidiNotes[index] = note;
        noteIsPlaying[index] = true;
        
        if (currentMode == MODE_DRUMS) {
          sendMidiNoteOn(drumChannel, note, 127);
          currentNote = drumNames[index % 10];
        } else {
          sendMidiNoteOn(midiChannel, note, 127);
          currentNote = getScaleNoteName(note); // Use correct function name
        }
        
        displayTimeout = millis() + DISPLAY_TIMEOUT;
        updateDisplay();
      } else { // Released
        if (noteIsPlaying[index]) {
          if (currentMode == MODE_DRUMS) {
            sendMidiNoteOff(drumChannel, playedMidiNotes[index], 0);
          } else {
            sendMidiNoteOff(midiChannel, playedMidiNotes[index], 0);
          }
          noteIsPlaying[index] = false;
        }
      }
    }
  }

  lastNoteStates[index] = reading;
}

// === DRUM BUTTON HANDLER ===
void handleDrumButton(int index, int pin) {
  int reading = digitalRead(pin);
  
  // Use a separate state tracking for extra drum buttons
  static bool extraDrumStates[3] = {HIGH, HIGH, HIGH};
  static bool lastExtraDrumStates[3] = {HIGH, HIGH, HIGH};
  
  int extraIndex = index - numNoteButtons; // 0, 1, or 2 for the extra buttons
  
  if (reading != lastExtraDrumStates[extraIndex]) {
    lastDebounceTime[index] = millis();
  }

  if ((millis() - lastDebounceTime[index]) > debounceDelay) {
    if (reading != extraDrumStates[extraIndex]) {
      extraDrumStates[extraIndex] = reading;

      if (extraDrumStates[extraIndex] == LOW) {
        int drumNote = calculateDrumMidiNote(index);
        playedMidiNotes[index] = drumNote;
        noteIsPlaying[index] = true;
        sendMidiNoteOn(drumChannel, drumNote, 127);
        currentNote = drumNames[index % 10];
        displayTimeout = millis() + DISPLAY_TIMEOUT;
        updateDisplay();
      } else {
        if (noteIsPlaying[index]) {
          sendMidiNoteOff(drumChannel, playedMidiNotes[index], 0);
          noteIsPlaying[index] = false;
        }
      }
    }
  }

  lastExtraDrumStates[extraIndex] = reading;
}

// === OCTAVE BUTTON HANDLERS (STANDARD MODE) ===
void handleOctaveButtons() {
  int downReading = digitalRead(octaveDownPin);
  int upReading = digitalRead(octaveUpPin);

  if (downReading != lastOctaveDownState) {
    lastDebounceTime[numNoteButtons + 1] = millis();
  }
  if (upReading != lastOctaveUpState) {
    lastDebounceTime[numNoteButtons + 2] = millis();
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 1]) > debounceDelay) {
    if (downReading != octaveDownState) {
      octaveDownState = downReading;
      if (octaveDownState == LOW) {
        octaveOffset = max(octaveOffset - 1, -3);
        updateDisplay();
      }
    }
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 2]) > debounceDelay) {
    if (upReading != octaveUpState) {
      octaveUpState = upReading;
      if (octaveUpState == LOW) {
        octaveOffset = min(octaveOffset + 1, 3);
        updateDisplay();
      }
    }
  }

  lastOctaveDownState = downReading;
  lastOctaveUpState = upReading;
}

// === TRANSPOSE BUTTON HANDLERS (SCALE MODE) ===
void handleTransposeButtons() {
  int downReading = digitalRead(octaveDownPin);
  int upReading = digitalRead(octaveUpPin);

  if (downReading != lastOctaveDownState) {
    lastDebounceTime[numNoteButtons + 1] = millis();
  }
  if (upReading != lastOctaveUpState) {
    lastDebounceTime[numNoteButtons + 2] = millis();
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 1]) > debounceDelay) {
    if (downReading != octaveDownState) {
      octaveDownState = downReading;
      if (octaveDownState == LOW) {
        semitoneOffset = max(semitoneOffset - 1, -24); // Down by semitone
        updateDisplay();
      }
    }
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 2]) > debounceDelay) {
    if (upReading != octaveUpState) {
      octaveUpState = upReading;
      if (octaveUpState == LOW) {
        semitoneOffset = min(semitoneOffset + 1, 24); // Up by semitone
        updateDisplay();
      }
    }
  }

  lastOctaveDownState = downReading;
  lastOctaveUpState = upReading;
}

// === SHARP BUTTON HANDLER (STANDARD MODE) ===
void handleSharpButton() {
  int reading = digitalRead(sharpPin);

  if (reading != lastSharpState) {
    lastDebounceTime[numNoteButtons + 4] = millis();
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 4]) > debounceDelay) {
    if (reading != sharpState) {
      sharpState = reading;
      updateDisplay();
    }
  }

  lastSharpState = reading;
}

// === SCALE BUTTON HANDLER (SCALE MODE) ===
void handleScaleButton() {
  int reading = digitalRead(sharpPin); // Reuse sharp button for scale selection

  if (reading != lastSharpState) {
    lastDebounceTime[numNoteButtons + 4] = millis();
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 4]) > debounceDelay) {
    if (reading != sharpState) {
      sharpState = reading;
      
      if (sharpState == LOW) { // Button pressed
        // Cycle through available scales
        currentScale = (ScaleType)((currentScale + 1) % SCALE_COUNT);
        updateDisplay();
      }
    }
  }

  lastSharpState = reading;
}

// === MODE SWITCH BUTTON HANDLER ===
void handleModeButton() {
  int reading = digitalRead(modePin);

  if (reading != lastModeState) {
    lastDebounceTime[numNoteButtons + 3] = millis();
  }

  if ((millis() - lastDebounceTime[numNoteButtons + 3]) > debounceDelay) {
    if (reading != modeState) {
      modeState = reading;

      if (modeState == LOW) {
        stopAllPlayingNotes();
        currentMode = (ControllerMode)((currentMode + 1) % MODE_COUNT);
        
        // Reset offsets when changing modes
        if (currentMode == MODE_STANDARD) {
          octaveOffset = 0;
        } else if (currentMode == MODE_SCALES) {
          semitoneOffset = 0;
        }
        
        updateDisplay();
      }
    }
  }

  lastModeState = reading;
}

// === STOP ALL PLAYING NOTES ===
// This function is defined in midi_functions.h to avoid redefinition

// === CHORD BUTTON HANDLER (CHORD MODE) ===
// Adicionar função para lidar com botões no modo chord
void handleChordButton(int index) {
  int reading = digitalRead(notePins[index]);

  if (reading != lastNoteStates[index]) {
    lastDebounceTime[index] = millis();
  }

  if ((millis() - lastDebounceTime[index]) > debounceDelay) {
    if (reading != noteStates[index]) {
      noteStates[index] = reading;

      if (noteStates[index] == LOW) { // Pressed
        int noteCount;
        int* chordNotes = calculateChordMidiNotes(index, &noteCount);
        
        // Tocar todas as notas do acorde
        for (int i = 0; i < noteCount; i++) {
          sendMidiNoteOn(midiChannel, chordNotes[i], 127);
        }
        
        // Armazenar informações para o note off
        playedMidiNotes[index] = index; // Usar index como identificador
        noteIsPlaying[index] = true;
        
        // Mostrar nome do acorde no display
        currentNote = "Chord " + String(index + 1);
        displayTimeout = millis() + DISPLAY_TIMEOUT;
        updateDisplay();
        
      } else { // Released
        if (noteIsPlaying[index]) {
          int noteCount;
          int* chordNotes = calculateChordMidiNotes(index, &noteCount);
          
          // Parar todas as notas do acorde
          for (int i = 0; i < noteCount; i++) {
            sendMidiNoteOff(midiChannel, chordNotes[i], 0);
          }
          
          noteIsPlaying[index] = false;
        }
      }
    }
  }

  lastNoteStates[index] = reading;
}

// === UPDATE ALL BUTTONS BASED ON CURRENT MODE ===
void updateButtons() {
  // Handle mode button (always active)
  handleModeButton();
  
  if (currentMode == MODE_STANDARD) {
    // Handle 7 note buttons
    for (int i = 0; i < numNoteButtons; i++) {
      handleNoteButton(i);
    }
    handleSharpButton();
    handleOctaveButtons();
    
  } else if (currentMode == MODE_SCALES) {
    // Handle 7 note buttons
    for (int i = 0; i < numNoteButtons; i++) {
      handleNoteButton(i);
    }
    handleScaleButton(); // Sharp button becomes scale selector
    handleTransposeButtons(); // Octave buttons become transpose
    
  } else if (currentMode == MODE_DRUMS) {
    // Handle all 10 buttons as drums (7 main + 3 extra)
    for (int i = 0; i < numNoteButtons; i++) {
      handleNoteButton(i); // These will play drums in drum mode
    }
    // Handle the 3 extra buttons (sharp, octave down, octave up) as drums
    handleDrumButton(numNoteButtons, sharpPin);     // Button 8
    handleDrumButton(numNoteButtons + 1, octaveDownPin); // Button 9
    handleDrumButton(numNoteButtons + 2, octaveUpPin);   // Button 10
  } else if (currentMode == MODE_CHORD) { // NOVO MODO
    for (int i = 0; i < numNoteButtons; i++) {
      handleChordButton(i);
    }
    // No modo chord, os botões extras podem ser usados para funções especiais
    // ou simplesmente ignorados
  }
}

#endif // BUTTON_HANDLERS_H