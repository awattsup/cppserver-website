#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <boost/log/trivial.hpp>
#include <json/json.h>
#include "database.hpp"
#include "devices.hpp"



void processPOSTData(const Json::Value& postJSON, DeviceList& deviceList) {
    std::cout << postJSON["temp"] << std::endl;

    std::string deviceName = postJSON["deviceName"].asString();
    int deviceID = postJSON["deviceID"].asInt();
    std::string statusFilename = "data/devices/" + std::to_string(deviceID) + "_" + deviceName + "_status.json";

    if (Device* device = deviceList.getDevice(deviceID)){
        // Device is in active device list, update data
        device -> updateDataFromPOST(postJSON);
    } else if (std::filesystem::exists(statusFilename)) {
        // Device not active but exists, so load it into the deviceList and update data
        if (deviceName.find("iSpindle") != std:string::npos) {
            iSpindle* isp = new iSpindle(statusFilename); // TODO implement constructor to load from file); 
        })
    } else {
        // Device does not exist, so create a new device

    }
    if 
    
    // std::filesystem::exists()
    // std::fi in("./public/" + filename, std::ifstream::in);
    // if (data/devices/" + std::to_string(deviceID) + "_" + deviceName + "_status.json")

    try
    {
        Device* device = deviceList.getDevice(deviceID);


    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    





}





