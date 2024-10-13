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

bool isTemperatureOk(float temperature) {
    if (temperature < MIN_TEMP || temperature > MAX_TEMP) {
        displayAlert("Temperature is critical!");
        return false;
    }
    return true;
}

bool isPulseRateOk(float pulseRate) {
    if (pulseRate < MIN_PULSE || pulseRate > MAX_PULSE) {
        displayAlert("Pulse Rate is out of range!");
        return false;
    }
    return true;
}

bool isSpo2Ok(float spo2) {
    if (spo2 < MIN_SPO2) {
        displayAlert("Oxygen Saturation out of range!");
        return false;
    }
    return true;
}

// Main function to check all vitals
bool vitalsOk(float temperature, float pulseRate, float spo2) {
    const bool tempOk = isTemperatureOk(temperature);
    const bool pulseOk = isPulseRateOk(pulseRate);
    const bool spo2Ok = isSpo2Ok(spo2);
    return tempOk && pulseOk && spo2Ok;
}
