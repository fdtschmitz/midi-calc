#include <unity.h>
#include <Arduino.h>
#include <MIDIUSB.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Include your headers
#include "config.h"
#include "modes.h"

// Mock global variables for testing
ControllerMode currentMode = MODE_STANDARD;
ScaleType currentScale = SCALE_MAJOR;
int currentOctave = 4;
int octaveOffset = 0;
int semitoneOffset = 0;

void setUp(void) {
    // Set up before each test
    currentMode = MODE_STANDARD;
    currentScale = SCALE_MAJOR;
    currentOctave = 4;
    octaveOffset = 0;
    semitoneOffset = 0;
}

void tearDown(void) {
    // Clean up after each test
}

void test_initial_mode_is_standard(void) {
    TEST_ASSERT_EQUAL(MODE_STANDARD, currentMode);
}

void test_initial_octave_is_4(void) {
    TEST_ASSERT_EQUAL(4, currentOctave);
}

void test_initial_scale_is_major(void) {
    TEST_ASSERT_EQUAL(SCALE_MAJOR, currentScale);
}

void test_mode_cycling(void) {
    // Test mode cycling logic
    currentMode = MODE_STANDARD;
    // Simulate mode button press logic
    ControllerMode nextMode = (ControllerMode)((currentMode + 1) % 3);
    TEST_ASSERT_EQUAL(MODE_SCALES, nextMode);
    
    currentMode = nextMode;
    nextMode = (ControllerMode)((currentMode + 1) % 3);
    TEST_ASSERT_EQUAL(MODE_DRUMS, nextMode);
    
    currentMode = nextMode;
    nextMode = (ControllerMode)((currentMode + 1) % 3);
    TEST_ASSERT_EQUAL(MODE_STANDARD, nextMode);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_initial_mode_is_standard);
    RUN_TEST(test_initial_octave_is_4);
    RUN_TEST(test_initial_scale_is_major);
    RUN_TEST(test_mode_cycling);
    UNITY_END();
}

void loop() {
    // Empty loop for Arduino compatibility
}