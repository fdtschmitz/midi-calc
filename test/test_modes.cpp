#include <unity.h>
#include <Arduino.h>
#include "modes.h"

// Test scale definitions
int majorScale[] = {0, 2, 4, 5, 7, 9, 11};
int minorScale[] = {0, 2, 3, 5, 7, 8, 10};
int dorianScale[] = {0, 2, 3, 5, 7, 9, 10};

int getScaleNote(ScaleType scale, int degree, int semitoneOffset = 0) {
    int* scaleArray;
    
    switch(scale) {
        case SCALE_MAJOR:
            scaleArray = majorScale;
            break;
        case SCALE_MINOR:
            scaleArray = minorScale;
            break;
        case SCALE_DORIAN:
            scaleArray = dorianScale;
            break;
        default:
            scaleArray = majorScale;
    }
    
    if (degree >= 0 && degree < 7) {
        return scaleArray[degree] + semitoneOffset;
    }
    return 0;
}

void setUp(void) {
    // Setup
}

void tearDown(void) {
    // Cleanup
}

void test_major_scale_notes(void) {
    TEST_ASSERT_EQUAL(0, getScaleNote(SCALE_MAJOR, 0));   // Root
    TEST_ASSERT_EQUAL(2, getScaleNote(SCALE_MAJOR, 1));   // Major 2nd
    TEST_ASSERT_EQUAL(4, getScaleNote(SCALE_MAJOR, 2));   // Major 3rd
    TEST_ASSERT_EQUAL(5, getScaleNote(SCALE_MAJOR, 3));   // Perfect 4th
    TEST_ASSERT_EQUAL(7, getScaleNote(SCALE_MAJOR, 4));   // Perfect 5th
    TEST_ASSERT_EQUAL(9, getScaleNote(SCALE_MAJOR, 5));   // Major 6th
    TEST_ASSERT_EQUAL(11, getScaleNote(SCALE_MAJOR, 6));  // Major 7th
}

void test_minor_scale_notes(void) {
    TEST_ASSERT_EQUAL(0, getScaleNote(SCALE_MINOR, 0));   // Root
    TEST_ASSERT_EQUAL(2, getScaleNote(SCALE_MINOR, 1));   // Major 2nd
    TEST_ASSERT_EQUAL(3, getScaleNote(SCALE_MINOR, 2));   // Minor 3rd
    TEST_ASSERT_EQUAL(5, getScaleNote(SCALE_MINOR, 3));   // Perfect 4th
    TEST_ASSERT_EQUAL(7, getScaleNote(SCALE_MINOR, 4));   // Perfect 5th
    TEST_ASSERT_EQUAL(8, getScaleNote(SCALE_MINOR, 5));   // Minor 6th
    TEST_ASSERT_EQUAL(10, getScaleNote(SCALE_MINOR, 6));  // Minor 7th
}

void test_scale_transposition(void) {
    // Test transposing major scale up 2 semitones
    TEST_ASSERT_EQUAL(2, getScaleNote(SCALE_MAJOR, 0, 2));   // Root + 2
    TEST_ASSERT_EQUAL(4, getScaleNote(SCALE_MAJOR, 1, 2));   // Major 2nd + 2
    TEST_ASSERT_EQUAL(6, getScaleNote(SCALE_MAJOR, 2, 2));   // Major 3rd + 2
}

void test_mode_enum_values(void) {
    TEST_ASSERT_EQUAL(0, MODE_STANDARD);
    TEST_ASSERT_EQUAL(1, MODE_SCALES);
    TEST_ASSERT_EQUAL(2, MODE_DRUMS);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_major_scale_notes);
    RUN_TEST(test_minor_scale_notes);
    RUN_TEST(test_scale_transposition);
    RUN_TEST(test_mode_enum_values);
    UNITY_END();
}

void loop() {}