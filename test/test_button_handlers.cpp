#include <unity.h>
#include <Arduino.h>

// Mock Arduino functions
unsigned long mockMillis = 0;
unsigned long millis() {
    return mockMillis;
}

void setMockMillis(unsigned long time) {
    mockMillis = time;
}

// Test debouncing logic
bool testDebounce(unsigned long lastTime, unsigned long currentTime, unsigned long debounceDelay = 50) {
    return (currentTime - lastTime) > debounceDelay;
}

void setUp(void) {
    mockMillis = 0;
}

void tearDown(void) {
    // Clean up
}

void test_debounce_logic_initial_press(void) {
    unsigned long lastDebounceTime = 0;
    setMockMillis(100);
    
    bool shouldProcess = testDebounce(lastDebounceTime, millis());
    TEST_ASSERT_TRUE(shouldProcess);
}

void test_debounce_logic_too_soon(void) {
    unsigned long lastDebounceTime = 100;
    setMockMillis(120); // Only 20ms later
    
    bool shouldProcess = testDebounce(lastDebounceTime, millis());
    TEST_ASSERT_FALSE(shouldProcess);
}

void test_debounce_logic_enough_time(void) {
    unsigned long lastDebounceTime = 100;
    setMockMillis(200); // 100ms later
    
    bool shouldProcess = testDebounce(lastDebounceTime, millis());
    TEST_ASSERT_TRUE(shouldProcess);
}

void test_button_state_change_detection(void) {
    bool currentState = LOW;  // Pressed (with pullup)
    bool lastState = HIGH;    // Not pressed
    
    bool stateChanged = (currentState != lastState);
    TEST_ASSERT_TRUE(stateChanged);
    
    // No change
    currentState = HIGH;
    lastState = HIGH;
    stateChanged = (currentState != lastState);
    TEST_ASSERT_FALSE(stateChanged);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_debounce_logic_initial_press);
    RUN_TEST(test_debounce_logic_too_soon);
    RUN_TEST(test_debounce_logic_enough_time);
    RUN_TEST(test_button_state_change_detection);
    UNITY_END();
}

void loop() {}