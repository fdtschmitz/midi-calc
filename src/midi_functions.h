/*
 * midi_functions.h - MIDI Communication Functions
 * 
 * This file contains all MIDI-related functions for sending notes,
 * calculating MIDI values, and handling MIDI communication.
 */

#ifndef MIDI_FUNCTIONS_H
#define MIDI_FUNCTIONS_H

// Function declarations
void sendMidiNoteOn(byte channel, byte note, byte velocity);
void sendMidiNoteOff(byte channel, byte note, byte velocity);
int calculateStandardMidiNote(int buttonIndex);
int calculateScaleMidiNote(int buttonIndex);
int calculateDrumMidiNote(int buttonIndex);
int* calculateChordMidiNotes(int buttonIndex, int* noteCount);
void stopAllPlayingNotes();
void stopAllChordNotes();

// External variables needed for MIDI functions
extern ControllerMode currentMode;
extern ScaleType currentScale;
extern bool sharpState;
extern int octaveOffset;
extern int semitoneOffset;
extern int playedMidiNotes[];
extern bool noteIsPlaying[];

void sendMidiNoteOn(byte channel, byte note, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, note, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void sendMidiNoteOff(byte channel, byte note, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, note, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

int calculateStandardMidiNote(int buttonIndex) {
  int midiNote = baseNotes[buttonIndex] + (octaveOffset * 12);
  
  // Add sharp if sharp button is held
  if (sharpState == LOW) {
    midiNote += 1;
  }
  
  return midiNote;
}

int calculateScaleMidiNote(int buttonIndex) {
  int rootNote = 60; // C4 (no octave offset for scales mode)
  int interval = scaleIntervals[currentScale][buttonIndex];
  
  // Apply semitone transposition
  return rootNote + interval + semitoneOffset;
}

int calculateDrumMidiNote(int buttonIndex) {
  // Return the appropriate drum note based on button index
  if (buttonIndex < 10) {
    return drumNotes[buttonIndex];
  }
  return drumNotes[0]; // Fallback to kick drum
}

int* calculateChordMidiNotes(int buttonIndex, int* noteCount) {
  static int midiNotes[8];
  *noteCount = 0;
  
  if (buttonIndex >= 7) return midiNotes;
  
  ChordData chord = chords[buttonIndex];
  *noteCount = chord.noteCount;
  
  int baseNote = 60 + (chord.octave - 4) * 12; // C4 como base
  
  for (int i = 0; i < chord.noteCount; i++) {
    midiNotes[i] = baseNote + chord.notes[i];
  }
  
  return midiNotes;
}

void stopAllPlayingNotes() {
  int maxButtons = (currentMode == MODE_DRUMS) ? 10 : 7;
  
  for (int i = 0; i < maxButtons; i++) {
    if (noteIsPlaying[i]) {
      int channel = (currentMode == MODE_DRUMS) ? drumChannel : midiChannel;
      sendMidiNoteOff(channel, playedMidiNotes[i], 0);
      noteIsPlaying[i] = false;
    }
  }
}

void stopAllChordNotes() {
  for (int i = 0; i < 7; i++) {
    if (noteIsPlaying[i]) {
      int noteCount;
      int* chordNotes = calculateChordMidiNotes(i, &noteCount);
      
      for (int j = 0; j < noteCount; j++) {
        sendMidiNoteOff(midiChannel, chordNotes[j], 0);
      }
      
      noteIsPlaying[i] = false;
    }
  }
}


#endif // MIDI_FUNCTIONS_H