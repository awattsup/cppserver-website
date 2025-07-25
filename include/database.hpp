#pragma once
#include <string>
#include <map>
#include <json/json.h>

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
    std::string hydrometerLog = std::to_string(brewID) + "_" + brewName + "_hydromlog.json"; // Log of hydrometer readings
public:
    Brew();

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
    Json::Value toJson() const;
    void fromJson(const Json::Value& val);
};

class BrewDatabase {
private:
    std::map<int, Brew> brews; // Map to store Brew objects
    std::string databaseFilename = "brewDatabase.json"; // Default log file name
public:
    BrewDatabase();

    void addBrew(const Brew& brew);
    Brew* getBrew(int id);
    void removeBrew(int id);
    size_t size() const;

    // Persistence
    void saveToFile() const;
    void loadFromFile();

    // JSON serialization
    Json::Value toJson() const;
    void fromJson(const Json::Value& root);

private:
    std::map<int, Brew> brews;
    std::string databaseFilename;
};