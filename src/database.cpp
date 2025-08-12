#include <cstdlib>
#include <fstream>
#include <ctime>
#include <boost/log/trivial.hpp>

#include "database.hpp"




Brew::Brew() : brewID(-1), IG(0.0), FG(0.0), ABV(0.0), volume(0.0), brewName(""), brewType(""), brewDate(""), brewStatus("") {
        // Constructor implementation
    };

// Add methods to handle brew data
void Brew::processData(const std::string& data) {
    // Process the incoming data
}


Json::Value Brew::toBrewJSON() const {
    Json::Value brewJSON;
    brewJSON["brewID"] = brewID;
    brewJSON["brewName"] = brewName;
    brewJSON["brewType"] = brewType;
    brewJSON["brewDate"] = brewDate;
    brewJSON["brewStatus"] = brewStatus;
    brewJSON["IG"] = IG;
    brewJSON["FG"] = FG;
    brewJSON["volume"] = volume;
    brewJSON["ABV"] = ABV;
    return brewJSON;
}

void Brew::fromBrewJSON(const Json::Value& brewJSON) {
    brewID = brewJSON["brewID"].asInt();
    brewName = brewJSON["brewName"].asString();
    brewType = brewJSON["brewType"].asString();
    brewDate = brewJSON["brewDate"].asString();
    brewStatus = brewJSON["brewStatus"].asString();
    IG = brewJSON["IG"].asDouble();
    FG = brewJSON["FG"].asDouble();
    volume = brewJSON["volume"].asDouble();
    ABV = brewJSON["ABV"].asDouble();
}



    
void Brew::updateBrewDataLog(const std::string& deviceType, const std::string& logData){
    // Update the brew log with device data
    logFilename = "data/logs/" + std::to_string(brewID) + "_" + brewName + "_" + deviceType + ".log"
    std::ofstream logFile(logFilename, std::ios::app);
    if (logFile.is_open()) {
        logFile << deviceType << "," << logData << std::endl;
        logFile.close();
    }
}


// Setters and getters for brew data
void Brew::setBrewID(int id) {
    brewID = id;
}

void Brew::setIG(double ig) {
    IG = ig;
}

void Brew::setFG(double fg) {
    FG = fg;
}

void Brew::setABV(double abv) {
    ABV = 131.5*(IG-FG);
}

void Brew::setVolume(double vol) {
    volume = vol;
}

void Brew::setBrewName(const std::string& name) {
    brewName = name;
}

void Brew::setBrewType(const std::string& type) {
    brewType = type;
}

void Brew::setBrewDate(const std::string& date) {
    brewDate = date;
}

void Brew::setBrewStatus(const std::string& status) {
    brewStatus = status;
}


int Brew::getBrewID() const {
    return brewID;
}

double Brew::getIG() const {
    return IG;
}

double Brew::getFG() const {
    return FG;
}   

double Brew::getABV() const {
    return ABV;
}

double Brew::getVolume() const {
    return volume;
}

std::string Brew::getBrewName() const {
    return brewName;
}

std::string Brew::getBrewType() const {
    return brewType;
}

std::string Brew::getBrewDate() const {
    return brewDate;
}

std::string Brew::getBrewStatus() const {
    return brewStatus;
}



BrewDatabase::BrewDatabase()  {
    if (std::filesystem::exists(databaseFilename)) {
        loadDatabaseFile();
    } else {
        BOOST_LOG_TRIVIAL(info) << "Brew database file not found, starting with an empty database.";
        std::ofstream outputFile(databaseFilename);
        outputFile << "{}"; // Create an empty JSON object
        outputFile.close();
    }
} 

void BrewDatabase::addBrew( Brew& brew)  {
    brew.setBrewID(nBrews + 1); // Assign a new ID
    brews[nBrews + 1] = brew;
}

Brew BrewDatabase::getBrew(int brewID) const {
    auto it = brews.find(brewID);
    if (it != brews.end()) {
        return it->second;
    }
    throw std::runtime_error("Brew not found");
}

void BrewDatabase::removeBrew(int brewID) {
    brews.erase(brewID);
}



// Serialize the map to JSON
Json::Value BrewDatabase::toDatabaseJSON() const {
    Json::Value root(Json::objectValue);
    for (const auto& [id, brew] : brews) {
        root[std::to_string(id)] = brew.toBrewJSON();
    }
    return root;
}

// Deserialize the map from JSON
void BrewDatabase::fromDatabaseJSON(const Json::Value& root) {
    brews.clear();
    for (const auto& key : root.getMemberNames()) {
        Brew brew;
        brew.fromBrewJSON(root[key]);
        int id = brew.getBrewID();
        brews[id] = brew;
    }
}


// Save to file
void BrewDatabase::saveDatabaseFile() const {
    std::ofstream file(databaseFilename);
    if (file.is_open()) {
        Json::StreamWriterBuilder writer;
        file << Json::writeString(writer, toDatabaseJSON());
        file.close();
    }
}

// Load from file
void BrewDatabase::loadDatabaseFile() {
    std::ifstream file(databaseFilename);
    if (file.is_open()) {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;
        bool ok = Json::parseFromStream(reader, file, &root, &errs);
        if (ok) fromDatabaseJSON(root);
        file.close();
    }
}
