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


class patientVitals {
public:
    virtual bool isOk(float value) const = 0;
    virtual void displayAlert(const std::string& message) const {
        std::cout << "ALERT: " << message << std::endl;
    }
    virtual ~patientVitals() = default;
};

template<typename T>
class Vital : public patientVitals {
private:
    float minLimit;
    float maxLimit;
    std::string alertMessage;
    std::string lowWarningMessage;
    std::string highWarningMessage;
    float tolerance;  // 1.5% tolerance for early warning

public:
    Vital(float minL, float maxL, const std::string& alertMsg, 
          const std::string& lowWarnMsg, const std::string& highWarnMsg, 
          float tol = 1.5f)
        : minLimit(minL), maxLimit(maxL), alertMessage(alertMsg),
          lowWarningMessage(lowWarnMsg), highWarningMessage(highWarnMsg),
          tolerance(tol) {}

    bool isOk(float value) const override {
    if (!isInCriticalRange(value)) {
        return false;
    }
    issueWarningIfNeeded(value);
    return true;
    }

    private:
    bool isInCriticalRange(float value) const {
    if (value < minLimit || value > maxLimit) {
        displayAlert(alertMessage);
        return false;
    }
    return true;
    }

    void issueWarningIfNeeded(float value) const {
    const float toleranceAmount = (tolerance / 100.0f) * maxLimit;
    const float lowerWarningThreshold = minLimit + toleranceAmount;  // Closer to min
    const float upperWarningThreshold = maxLimit - toleranceAmount;

        if (value >= minLimit && value <= lowerWarningThreshold) {
            displayAlert(lowWarningMessage); // Approaching lower limit
        }
        if (value >= upperWarningThreshold && value <= maxLimit) {
            displayAlert(highWarningMessage); // Approaching upper limit
        }
    }
};

class Temperature : public Vital<Temperature> {
public:
    Temperature() : Vital(MIN_TEMP, MAX_TEMP, 
                          "Temperature is critical!", 
                          "Warning: Approaching hypothermia", 
                          "Warning: Approaching hyperthermia") {}
};

class PulseRate : public Vital<PulseRate> {
public:
    PulseRate() : Vital(MIN_PULSE, MAX_PULSE, 
                        "Pulse Rate is out of range!", 
                        "Warning: Approaching bradycardia", 
                        "Warning: Approaching tachycardia") {}
};

class SpO2 : public Vital<SpO2> {
public:
    SpO2() : Vital(MIN_SPO2, 100.0f, 
                   "Oxygen Saturation is critical!", 
                   "Warning: Approaching hypoxemia", 
                   "Warning: Approaching hyperoxia") {}
};



bool vitalsOk(float temperature, float pulseRate, float spo2) {
    Temperature tempCheck;
    const bool tempOk = tempCheck.isOk(temperature);

    PulseRate pulseCheck;
    const bool pulseOk = pulseCheck.isOk(pulseRate);

    SpO2 spo2Check;
    const bool spo2Ok = spo2Check.isOk(spo2);

    return tempOk && pulseOk && spo2Ok;
}