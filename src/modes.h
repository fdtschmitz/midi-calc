/*
 * modes.h - Mode Definitions
 * 
 * This file contains all mode-related definitions, enums, and data structures
 * for the different controller modes.
 */

#ifndef MODES_H
#define MODES_H

// Controller modes
enum ControllerMode {
  MODE_STANDARD = 0,
  MODE_SCALES = 1,
  MODE_DRUMS = 2,
  MODE_COUNT = 3
};

extern String modeNames[3];

// Scale types for scales mode
enum ScaleType {
  SCALE_MAJOR = 0,
  SCALE_MINOR = 1,
  SCALE_HARMONIC_MINOR = 2,
  SCALE_MELODIC_MINOR = 3,
  SCALE_DORIAN = 4,
  SCALE_PHRYGIAN = 5,
  SCALE_LYDIAN = 6,
  SCALE_MIXOLYDIAN = 7,
  SCALE_LOCRIAN = 8,
  SCALE_PENTATONIC_MAJOR = 9,
  SCALE_PENTATONIC_MINOR = 10,
  SCALE_COUNT = 11
};

extern String scaleNames[11];

// Scale intervals (semitones from root)
extern const int scaleIntervals[11][7];

// Drums mode - MIDI notes for all 10 buttons
extern const int drumNotes[10];
extern const String drumNames[10];

// Initialize mode names
String modeNames[] = {"Standard", "Scales", "Drums"};

// Initialize scale names
String scaleNames[] = {
  "Major", "Minor", "Harmonic", "Melodic", "Dorian", 
  "Phrygian", "Lydian", "Mixolydian", "Locrian",
  "Pent.Maj", "Pent.Min"
};

// Scale intervals (semitones from root)
const int scaleIntervals[11][7] = {
  {0, 2, 4, 5, 7, 9, 11},    // Major
  {0, 2, 3, 5, 7, 8, 10},    // Minor (Natural)
  {0, 2, 3, 5, 7, 8, 11},    // Harmonic Minor
  {0, 2, 3, 5, 7, 9, 11},    // Melodic Minor (ascending)
  {0, 2, 3, 5, 7, 9, 10},    // Dorian
  {0, 1, 3, 5, 7, 8, 10},    // Phrygian
  {0, 2, 4, 6, 7, 9, 11},    // Lydian
  {0, 2, 4, 5, 7, 9, 10},    // Mixolydian
  {0, 1, 3, 5, 6, 8, 10},    // Locrian
  {0, 2, 4, 7, 9, 12, 14},   // Pentatonic Major (extended)
  {0, 3, 5, 7, 10, 12, 15}   // Pentatonic Minor (extended)
};

// Drums mode - MIDI notes for all 10 buttons
const int drumNotes[10] = {
  36, // Kick Drum
  38, // Snare Drum
  42, // Hi-hat Closed
  46, // Hi-hat Open
  49, // Crash Cymbal
  51, // Ride Cymbal
  53, // Bell
  35, // Kick Drum 2
  40, // Snare Drum 2
  44  // Hi-hat Pedal
};

const String drumNames[10] = {
  "Kick", "Snare", "HHat", "Open", "Crash", 
  "Ride", "Bell", "Kick2", "Snr2", "Pedal"
};

#endif // MODES_H