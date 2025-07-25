#include <cstdlib>
#include <string>
#include <cmath>

#include <json/json.h>

#include 'devices.hpp'

#include <map>
#include <memory>



iSpindle::iSpindle() = default; //TODO add constructor to initialize iSpindle data


// Deserialize the iSpindle data from JSON
void iSpindle::fromDeviceJSON(const JSON::Value& deviceJSON) {
    deviceID = deviceJSON["deviceId"].asString();
    temperature = deviceJSON["temperature"].asDouble();
    gravity = deviceJSON["gravity"].asDouble();
    angle = deviceJSON["angle"].asDouble();
    batteryVoltage = deviceJSON["batteryVoltage"].asDouble();
    calibGravity = deviceJSON["calibGravity"].asDouble();
    assignedBrewId = deviceJSON["assignedBrewId"].asInt(); 
    for (int i = 0; i <= 3; i++) {
        calibCoeffs[i] = deviceJSON["calibCoeffs"][i].asDouble();
    }
}


// Serialize the iSpindle data to JSON
void iSpindle::toDeviceJSON() {
    JSON::Value deviceJSON;
    deviceJSON["deviceId"] = deviceID;
    deviceJSON["temperature"] = temperature;
    deviceJSON["gravity"] = gravity;
    deviceJSON["angle"] = angle;
    deviceJSON["batteryVoltage"] = batteryVoltage;
    deviceJSON["calibGravity"] = calibGravity;
    deviceJSON["assignedBrewId"] = assignedBrewId;
    for (int i = 0; i <= 3; i++) {
        deviceJSON["calibCoeffs"][i] = calibCoeffs[i];
    }
    return deviceJSON;
}



// Add methods to handle iSpindle data
void iSpindle::processData(const std::string& data) {
    // Process the incoming data
}

// Setters and getters for iSpindle data 

void iSpindle::setTemperature(double temp) {
    temperature = temp;
}

void iSpindle::setGravity(double grav) {
    gravity = grav;
}

void iSpindle::setAngle(double ang) {
    angle = ang;
}   

void iSpindle::etBatteryVoltage(double voltage) {
    batteryVoltage = voltage;
}

void iSpindle::setCalibCoeffs(const double coeffs[4]) {
    for (int i = 0; i <= 3; i++) {
        calibCoeffs[i] = coeffs[i];
    }
}

void iSpindle::setCalibGravity(double calibGrav, const double calibCoeffs[4]) {
    calibGravity = 0; 
    for (int i = 0; i <= 3; i++) {
        calibGravity += calibCoeffs[i] * pow(angle, i);
    };
}

void iSpindle::setAssignedBrewId(int brewId) {
    assignedBrewId = brewId;
}

double iSpindle::getTemperature() const {
    return temperature;
}

double iSpindle::getGravity() const {
    return gravity;
}

double iSpindle::getAngle() const {
    return angle;
}

double iSpindle::getBatteryVoltage() const {
    return batteryVoltage;
}

double iSpindle::getCalibGravity() const {
    return calibGravity;
}

double iSpindle::getCalibCoeffs(int index) const {
    if (index < 0 || index >= 4) {
        throw std::out_of_range("Index out of range for calibration coefficients");
    }
    return calibCoeffs[index];
}

int iSpindle::getAssignedBrewId() const {
    return assignedBrewId;
}





DeviceList::DeviceList() {
    // Construct with just an empty map}

// Add a device (takes ownership)
void DeviceList::addDevice(std::unique_ptr<Device> device) {
    devices[device->getDeviceId()] = std::move(device);
}

// Get a device by ID (nullptr if not found)
Device* DeviceList::getDevice(const std::string& id) const {
    auto it = devices.find(id);
    if (it != devices.end()) {
        return it->second.get();
    }
    return nullptr;
}

// Remove a device by ID
void DeviceList::removeDevice(const std::string& id) {
    devices.erase(id);
}



