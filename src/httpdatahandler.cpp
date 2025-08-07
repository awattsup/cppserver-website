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

    if device.getAssignedBrewId() != -1) {

    




}





