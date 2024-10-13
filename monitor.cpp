#include "./monitor.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

const float MIN_TEMP = 95.0;
const float MAX_TEMP = 102.0;
const float MIN_PULSE = 60.0;
const float MAX_PULSE = 100.0;
const float MIN_SPO2 = 90.0;

void displayAlert(const std::string& message) {
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

bool isVitalOk(float value, float minLimit, float maxLimit, const std::string& alertMessage) {
    if (value < minLimit || value > maxLimit) {
        displayAlert(alertMessage);
        return false;
    }
    return true;
}

bool isTemperatureOk(float temperature) {
    return isVitalOk(temperature, MIN_TEMP, MAX_TEMP, "Temperature is critical!");
}

bool isPulseRateOk(float pulseRate) {
    return isVitalOk(pulseRate, MIN_PULSE, MAX_PULSE, "Pulse Rate is out of range!");
}

bool isSpo2Ok(float spo2) {
    return isVitalOk(spo2, MIN_SPO2, 100.0f, "Oxygen Saturation out of range!");
}

// Main function to check all vitals
bool vitalsOk(float temperature, float pulseRate, float spo2) {
    const bool tempOk = isTemperatureOk(temperature);
    const bool pulseOk = isPulseRateOk(pulseRate);
    const bool spo2Ok = isSpo2Ok(spo2);
    return tempOk && pulseOk && spo2Ok;
}
