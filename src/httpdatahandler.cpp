#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <boost/log/trivial.hpp>
#include <json/json.h>
#include "database.hpp"
#include "devices.hpp"



void processPOSTData(const Json::Value& postJSON, DeviceList& deviceList, BrewDatabase& brewDatabase) {
    std::cout << postJSON["temp"] << std::endl;

    std::string deviceName = postJSON["deviceName"].asString();
    int deviceID = postJSON["deviceID"].asInt();
    std::string statusFilename = "data/devices/" + std::to_string(deviceID) + "_" + deviceName + "_status.json";

    if (Device* device = deviceList.getDevice(deviceID)){
        // Device is in active device list, update data
        device -> updateDataFromPOST(postJSON);

    } else if (std::filesystem::exists(statusFilename)) {
        // Device not active but exists, so load it into the deviceList and update data
        if (deviceName.find("iSpindle") != std:string::npos) 
            iSpindle* device = new iSpindle(statusFilename); // TODO implement constructor to load from file
        
        device->updateDataFromPOST(postJSON); // update device with received POST data
        deviceList.addDevice(std::unique_ptr<Device>(device)); // add device to device list
        BOOST_LOG_TRIVIAL(info) << "Device " << deviceName << " loaded from status file and updated.";
    } else {
        // Device does not exist so make a new one
        if (postJSON["deviceName"].find("iSpindle") != std:string::npos) 
            iSpindle* device = new iSpindle(postJSON); // Create a new iSpindle from POST data

        deviceList.addDevice(std::unique_ptr<Device>(device)); // Add the new device to the device list
        BOOST_LOG_TRIVIAL(info) << "Device " << deviceName << " created from POST data.";
    }

    if (device->getAssignedBrewID() != -1) {
        // Device-indepdent logging logic
        // Example: data/logs/{brewID}_{brewName}_{logType}_{deviceID}.log
        int brewID = device->getAssignedBrewID();
        Brew& brew = brewDatabase[brewID];
        std::string logType = "hydrometer"; // or device->getLogType() if implemented.   //TODO change to device type attribute
        std::string logFilename = "data/logs/" + std::to_string(brewID) + "_" + brew.getBrewName() + "_" + logType + "_" + std::to_string(device->getDeviceID()) + ".log";

        // Prepare log data (CSV or JSON as needed)
        //TODO implement device methods for producing log data
        std::ofstream logFile(logFilename, std::ios::app);
        if (logFile.is_open()) {
            logFile << device->getTemperature() << ","
                    << device->getGravity() << ","
                    << device->getBatteryVoltage() << ","
                    << device->getRSSI() << std::endl;
            logFile.close();
        }
        BOOST_LOG_TRIVIAL(info) << "Logged data for device " << deviceName << " to " << logFilename;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Device " << deviceName << " is not assigned to any brew, no logs updated.";
    }


