#pragma once

#include <json/json.h>
#include "devices.hpp"
#include "database.hpp"

// Processes incoming POST data from a device, updates device list and brew database as needed.
// - postJSON: The parsed JSON from the POST request.
// - deviceList: The list of active devices (may be updated).
// - brewDatabase: The database of brews (may be updated).
void processPOSTData(const Json::Value& postJSON, DeviceList& deviceList, BrewDatabase& brewDatabase);