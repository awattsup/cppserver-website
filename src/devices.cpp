#include <cstdlib>
#include <cmath>
#include <fstream>

#include "devices.hpp"


Device::Device() = default; // Default constructor

Device::~Device() = default; // Default destructor


void Device::setDeviceName(const std::string& name){
    deviceName = name;
};

std::string Device::getDeviceName() const{
    return deviceName;
};

void Device::setDeviceID(const int& id){
    deviceID = id;
};

int Device::getDeviceID() const{
    return deviceID;
};

std::string Device::getStatusFileName() const{
    return statusfileName;
};




iSpindle::iSpindle() = default; //TODO add constructor to initialize iSpindle data

// Deserialize the iSpindle data from JSON 
void iSpindle::fromStatusfileJSON(const Json::Value& statusJSON) {
    deviceName = statusJSON["deviceName"].asString();
    temperature = statusJSON["temperature"].asDouble();
    gravity = statusJSON["gravity"].asDouble();
    angle = statusJSON["angle"].asDouble();
    batteryVoltage = statusJSON["batteryVoltage"].asDouble();
    calibGravity = statusJSON["calibGravity"].asDouble();
    RSSI = statusJSON["RSSI"].asInt();
    assignedBrewId = statusJSON["assignedBrewId"].asInt(); 
    for (int i = 0; i <= 3; i++) {
        calibCoeffs[i] = statusJSON["calibCoeffs"][i].asDouble();
    }
}


// Serialize the iSpindle data to JSON
Json::Value iSpindle::toStatusfileJSON() const {
    Json::Value statusJSON;
    statusJSON["deviceName"] = deviceName;
    statusJSON["temperature"] = temperature;
    statusJSON["gravity"] = gravity;
    statusJSON["angle"] = angle;
    statusJSON["batteryVoltage"] = batteryVoltage;
    statusJSON["calibGravity"] = calibGravity;
    statusJSON["assignedBrewId"] = assignedBrewId;
    for (int i = 0; i <= 3; i++) {
        statusJSON["calibCoeffs"][i] = calibCoeffs[i];
    }
    return statusJSON;
}



// iSpindle status file IO

// Save to file
void iSpindle::saveStatusfile() const {
    std::ofstream file(statusfileName);
    if (file.is_open()) {
        Json::StreamWriterBuilder writer;
        file << Json::writeString(writer, toStatusfileJSON());
        file.close();
    }
}

// Load from file
void iSpindle::loadStatusfile() {
    std::ifstream file(statusfileName);
    if (file.is_open()) {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;
        bool ok = Json::parseFromStream(reader, file, &root, &errs);
        if (ok) fromStatusfileJSON(root);
        file.close();
    }
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

void iSpindle::setBatteryVoltage(double voltage) {
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

void iSpindle::setRSSI(int rssi) {
    RSSI = rssi;
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

int iSpindle::getRSSI() const {
    return RSSI;
}

int iSpindle::getAssignedBrewId() const {
    return assignedBrewId;
}





DeviceList::DeviceList(){
    // Construct with just an empty map}
}

// Add a device (takes ownership)
void DeviceList::addDevice(std::unique_ptr<Device> device) {
    devices[device->getDeviceName()] = std::move(device);
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



