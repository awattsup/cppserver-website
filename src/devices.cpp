
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

void Device::setRSSI(int rssi) {
    RSSI = rssi;
}

void Device::setAssignedBrewID(int brewID) {
    assignedBrewID = brewID;
}

int Device::getDeviceID() const{
    return deviceID;
};

std::string Device::getStatusFileName() const{
    return statusfileName;
};


int Device::getRSSI() const {
    return RSSI;
}

int Device::getAssignedBrewID() const {
    return assignedBrewID;
}

std::string Device::getDeviceType() const {
    return deviceType;
}






iSpindle::iSpindle() = default;  //TODO Do I need to delete this? probably unused

iSpindle::iSpindle(std::string statusfilePath) {
    // constructor to initialize iSpindle from status file
    // Load from file
    std::ifstream file(statusfileName);
    if (file.is_open()) {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;
        bool ok = Json::parseFromStream(reader, file, &root, &errs);
        if (ok) fromStatusfileJSON(root);
        file.close();
    }
    statusfileName = statusfilePath; // Set the status file name
};

iSpindle::iSpindle(Json::Value postJSON) {
    //constructor to initialize iSpindle from postJSON
    deviceName = postJSON["name"].asString();
    deviceID = postJSON["ID"].asInt();
    temperature = postJSON["temperature"].asDouble();
    gravity = postJSON["gravity"].asDouble();
    angle = postJSON["angle"].asDouble();
    batteryVoltage = postJSON["battery"].asDouble();
    RSSI = postJSON["RSSI"].asInt();
    std::string deviceType = "hydrometer";
    statusfileName = "data/devices/" + std::to_string(deviceID) + "_" + deviceName + "_status.json"; // Set the status file name

};

// Deserialize the iSpindle data from JSON 
void iSpindle::fromStatusfileJSON(const Json::Value& statusJSON) {
    deviceName = statusJSON["deviceName"].asString();
    deviceID = statusJSON["deviceID"].asInt();
    temperature = statusJSON["temperature"].asDouble();
    gravity = statusJSON["gravity"].asDouble();
    angle = statusJSON["angle"].asDouble();
    batteryVoltage = statusJSON["batteryVoltage"].asDouble();
    calibGravity = statusJSON["calibGravity"].asDouble();
    RSSI = statusJSON["RSSI"].asInt();
    assignedBrewID = statusJSON["assignedBrewID"].asInt(); 
    for (int i = 0; i <= 3; i++) {
        calibCoeffs[i] = statusJSON["calibCoeffs"][i].asDouble();
    }
    statusfileName = "data/devices/" + std::to_string(deviceID) + "_" + deviceName + "_status.json"; // Default status file name
    deviceType = "hydrometer";
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
    statusJSON["assignedBrewID"] = assignedBrewID;
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
void iSpindle::updateDataFromPOST(const Json::Value& postJSON) {
    // Update iSpindle with the received POST data
    temperature = postJSON["temperature"].asDouble();
    gravity = postJSON["gravity"].asDouble();
    angle = postJSON["angle"].asDouble();
    batteryVoltage = postJSON["batteryVoltage"].asDouble();
    RSSI = postJSON["RSSI"].asInt();
    computeCalibGravity();
}

// Return data string for brew log
std::string iSpindle::getLogData() const {
    std::string logData;

    double grav = gravity; // Default to gravity
    if (calibGravity != 0) 
        grav = calibGravity;

    logData = getTimeDate() + ',' +
            std::to_string(temperature) + ',' +
            std::to_string(grav) + ',' +
            std::to_string(batteryVoltage) + ',' +
            std::to_string(RSSI);
   return logData;
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

void iSpindle::computeCalibGravity() {
    calibGravity = 0; 
    for (int i = 0; i <= 3; i++) {
        calibGravity += calibCoeffs[i] * pow(angle, i);
    };
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







DeviceList::DeviceList(){
    // Construct with just an empty map}
}

// Add a device (takes ownership)
void DeviceList::addDevice(std::unique_ptr<Device> device) {
    devices[device->getDeviceID()] = std::move(device);
}

// Get a device by ID (nullptr if not found)
Device* DeviceList::getDevice(const int id) const {
    auto it = devices.find(id);
    if (it != devices.end()) {
        return it->second.get();
    }
    return nullptr;
}

// Remove a device by ID
void DeviceList::removeDevice(const int id) {
    devices.erase(id);
}


// Serialize all devices to JSON for REST API
Json::Value DeviceList::toJson() const {
    Json::Value devicesJson(Json::arrayValue);
    for (const auto& [id, devicePtr] : devices) {
        Json::Value devJson;
        devJson["deviceID"] = devicePtr->getDeviceID();
        devJson["deviceName"] = devicePtr->getDeviceName();
        devJson["deviceType"] = devicePtr->getDeviceType();
        devJson["assignedBrewID"] = devicePtr->getAssignedBrewID();
        devJson["RSSI"] = devicePtr->getRSSI();
        devJson["statusfileName"] = devicePtr->getStatusFileName();
        devicesJson.append(devJson);
    }
    return devicesJson;
}



std::string getTimeDate() {
    const auto now = std::chrono::system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", now);
};