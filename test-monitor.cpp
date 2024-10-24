#include "./monitor.h"
#include <iostream>
#include <string>
#include "gtest/gtest.h"

TEST(Monitor, TemperatureBelowMinimum) {
    ASSERT_FALSE(vitalsOk(94.9, 70, 98));
}

TEST(Monitor, TemperatureAtMinimum) {
    ASSERT_TRUE(vitalsOk(95.0, 70, 98));
}

TEST(Monitor, TemperatureAtMaximum) {
    ASSERT_TRUE(vitalsOk(102.0, 70, 98));
}

TEST(Monitor, TemperatureAboveMaximum) {
    ASSERT_FALSE(vitalsOk(102.1, 70, 98));
}

TEST(Monitor, PulseRateBelowMinimum) {
    ASSERT_FALSE(vitalsOk(98.0, 59.9, 98));
}

TEST(Monitor, PulseRateAtMinimum) {
    ASSERT_TRUE(vitalsOk(98.0, 60.0, 98));
}

TEST(Monitor, PulseRateAtMaximum) {
    ASSERT_TRUE(vitalsOk(98.0, 100.0, 98));
}

TEST(Monitor, PulseRateAboveMaximum) {
    ASSERT_FALSE(vitalsOk(98.0, 100.1, 98));
}

TEST(Monitor, Spo2BelowMinimum) {
    ASSERT_FALSE(vitalsOk(98.0, 70, 89.9));
}

TEST(Monitor, Spo2AtMinimum) {
    ASSERT_TRUE(vitalsOk(98.0, 70, 90.0));
}

TEST(Monitor, VitalsAtExactThresholds) {
    ASSERT_TRUE(vitalsOk(95.0, 60.0, 90.0));
    ASSERT_TRUE(vitalsOk(102.0, 100.0, 90.0));
}

TEST(Monitor, MultipleVitalsOutOfRange) {
    ASSERT_FALSE(vitalsOk(94.9, 59.9, 89.9));
    ASSERT_FALSE(vitalsOk(102.1, 100.1, 90.1));
    ASSERT_FALSE(vitalsOk(96.0, 70.0, 85.0));
}

TEST(Monitor, VitalsCloseToThresholds) {
    ASSERT_TRUE(vitalsOk(95.1, 60.1, 90.1));
    ASSERT_TRUE(vitalsOk(101.9, 99.9, 90.0));
}

// Function to capture output from std::cout
std::string captureOutput(std::function<void()> func) {
    std::stringstream buffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    // Call the function to capture its output
    func();

    // Restore std::cout to its original buffer
    std::cout.rdbuf(oldCoutBuffer);

    return buffer.str();
}

TEST(Monitor, TemperatureBelowMinimumAlert) {
    std::string output = captureOutput([&]() {
        ASSERT_FALSE(vitalsOk(94.9, 70, 98));
    });
    ASSERT_EQ(output, "ALERT: Temperature is critical!\n");
}

TEST(Monitor, TemperatureAtMinimumAlert) {
    std::string output = captureOutput([&]() {
        ASSERT_TRUE(vitalsOk(95.0, 70, 98));
    });
    ASSERT_EQ(output, "ALERT: Warning: Approaching hypothermia\n");
}

TEST(Monitor, TemperatureAtMaximumAlert) {
    std::string output = captureOutput([&]() {
        ASSERT_TRUE(vitalsOk(102.0, 70, 98));
    });
    ASSERT_EQ(output, "ALERT: Warning: Approaching hyperthermia\n");
}
