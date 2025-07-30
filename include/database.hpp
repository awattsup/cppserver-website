#pragma once

#include <map>
#include <string>
#include <json/json.h> // JsonCpp library header


class Brew {
private:
    int brewID;
    double IG;
    double FG; 
    double ABV;
    double volume;
    std::string brewName;
    std::string brewType;
    std::string brewDate;
    std::string brewStatus;
    std::string hydrometerLog = "data/logs/" + std::to_string(brewID) + "_" + brewName + "_hydromlog.json"; // Log of hydrometer readings
public:
    Brew();

    void processData(const std::string& data);

    // Getters
    int getBrewID() const;
    double getIG() const;
    double getFG() const;
    double getABV() const;
    double getVolume() const;
    std::string getBrewName() const;
    std::string getBrewType() const;
    std::string getBrewDate() const;
    std::string getBrewStatus() const;

    // Setters
    void setBrewID(int id);
    void setIG(double ig);
    void setFG(double fg);
    void setABV(double abv);
    void setVolume(double vol);
    void setBrewName(const std::string& name);
    void setBrewType(const std::string& type);
    void setBrewDate(const std::string& date);
    void setBrewStatus(const std::string& status);

    // JSON serialization
    Json::Value toBrewJSON() const;
    void fromBrewJSON(const Json::Value& val);
};

class BrewDatabase {
private:
    std::map<int, Brew> brews; // Map to store Brew objects
    std::string databaseFilename = "data/brewDatabase.json"; // Default log file name
    int nBrews = brews.size(); // Number of brews in the database
public:
    BrewDatabase();

    void addBrew(Brew& brew) ;
    Brew getBrew(int id) const;
    void removeBrew(int id);
    size_t size() const;

    // Persistence
    void saveDatabaseFile() const;
    void loadDatabaseFile();

    // JSON serialization
    Json::Value toDatabaseJSON() const;
    void fromDatabaseJSON(const Json::Value& root);
};