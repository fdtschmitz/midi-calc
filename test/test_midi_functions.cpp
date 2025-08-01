#include <unity.h>
#include <Arduino.h>

// Mock MIDI functions for testing
class MockMIDI {
public:
    static int lastNote;
    static int lastVelocity;
    static int lastChannel;
    static bool noteOnCalled;
    static bool noteOffCalled;
    
    static void noteOn(byte channel, byte pitch, byte velocity) {
        lastChannel = channel;
        lastNote = pitch;
        lastVelocity = velocity;
        noteOnCalled = true;
    }
    
    static void noteOff(byte channel, byte pitch, byte velocity) {
        lastChannel = channel;
        lastNote = pitch;
        lastVelocity = velocity;
        noteOffCalled = true;
    }
    
    static void reset() {
        lastNote = -1;
        lastVelocity = -1;
        lastChannel = -1;
        noteOnCalled = false;
        noteOffCalled = false;
    }
};

int MockMIDI::lastNote = -1;
int MockMIDI::lastVelocity = -1;
int MockMIDI::lastChannel = -1;
bool MockMIDI::noteOnCalled = false;
bool MockMIDI::noteOffCalled = false;

// Helper function to calculate MIDI note
int calculateMIDINote(int noteIndex, int octave, bool sharp) {
    int baseNotes[] = {60, 62, 64, 65, 67, 69, 71}; // C, D, E, F, G, A, B in octave 4
    int note = baseNotes[noteIndex] + (octave - 4) * 12;
    if (sharp && noteIndex != 2 && noteIndex != 6) { // E and B don't have sharps
        note += 1;
    }
    return note;
}

void setUp(void) {
    MockMIDI::reset();
}

void tearDown(void) {
    // Clean up
}

void test_midi_note_calculation_c_major(void) {
    // Test C major scale notes
    TEST_ASSERT_EQUAL(60, calculateMIDINote(0, 4, false)); // C4
    TEST_ASSERT_EQUAL(62, calculateMIDINote(1, 4, false)); // D4
    TEST_ASSERT_EQUAL(64, calculateMIDINote(2, 4, false)); // E4
    TEST_ASSERT_EQUAL(65, calculateMIDINote(3, 4, false)); // F4
    TEST_ASSERT_EQUAL(67, calculateMIDINote(4, 4, false)); // G4
    TEST_ASSERT_EQUAL(69, calculateMIDINote(5, 4, false)); // A4
    TEST_ASSERT_EQUAL(71, calculateMIDINote(6, 4, false)); // B4
}

void test_midi_note_calculation_with_sharps(void) {
    // Test sharp notes
    TEST_ASSERT_EQUAL(61, calculateMIDINote(0, 4, true));  // C#4
    TEST_ASSERT_EQUAL(63, calculateMIDINote(1, 4, true));  // D#4
    TEST_ASSERT_EQUAL(64, calculateMIDINote(2, 4, true));  // E4 (no sharp)
    TEST_ASSERT_EQUAL(66, calculateMIDINote(3, 4, true));  // F#4
    TEST_ASSERT_EQUAL(68, calculateMIDINote(4, 4, true));  // G#4
    TEST_ASSERT_EQUAL(70, calculateMIDINote(5, 4, true));  // A#4
    TEST_ASSERT_EQUAL(71, calculateMIDINote(6, 4, true));  // B4 (no sharp)
}

void test_octave_changes(void) {
    // Test octave 3
    TEST_ASSERT_EQUAL(48, calculateMIDINote(0, 3, false)); // C3
    
    // Test octave 5
    TEST_ASSERT_EQUAL(72, calculateMIDINote(0, 5, false)); // C5
    
    // Test octave 6
    TEST_ASSERT_EQUAL(84, calculateMIDINote(0, 6, false)); // C6
}

void test_mock_midi_functions(void) {
    MockMIDI::noteOn(1, 60, 127);
    TEST_ASSERT_TRUE(MockMIDI::noteOnCalled);
    TEST_ASSERT_EQUAL(1, MockMIDI::lastChannel);
    TEST_ASSERT_EQUAL(60, MockMIDI::lastNote);
    TEST_ASSERT_EQUAL(127, MockMIDI::lastVelocity);
    
    MockMIDI::noteOff(1, 60, 0);
    TEST_ASSERT_TRUE(MockMIDI::noteOffCalled);
    TEST_ASSERT_EQUAL(0, MockMIDI::lastVelocity);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_midi_note_calculation_c_major);
    RUN_TEST(test_midi_note_calculation_with_sharps);
    RUN_TEST(test_octave_changes);
    RUN_TEST(test_mock_midi_functions);
    UNITY_END();
}

void loop() {}