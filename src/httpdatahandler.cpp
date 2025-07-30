#include <cstdlib>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <json/json.h>
#include "database.hpp"
#include "devices.hpp"



void processPOSTData(const Json::Value& postJSON, DeviceList& dl) {
    std::cout << postJSON["temp"] << std::endl;

    




}





