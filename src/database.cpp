#include <cstdlib>
#include <string>
#include <iofstream>
#include <json/json.h> // JsonCpp library header

#include <boost/log/trivial.hpp>


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
    std::string hydrometerLog = std::tostring(brewID) + brewName + "hydromlog.json"; // Log of hydrometer readings

    // std::string logfileName = std::tostring(brewID) + brewName".json"; // Default log file name



public:
    Brew() = default;

    // Add methods to handle brew data
    void processData(const std::string& data) {
        // Process the incoming data
    }


    Json::Value toJson() const {
        JSON::Value jsonBrew;
        jsonBrew["brewID"] = brewID;
        jsonBrew["brewName"] = brewName;
        jsonBrew["brewType"] = brewType;
        jsonBrew["brewDate"] = brewDate;
        jsonBrew["brewStatus"] = brewStatus;
        jsonBrew["IG"] = IG;
        jsonBrew["FG"] = FG;
        jsonBrew["volume"] = volume;
        jsonBrew["ABV"] = ABV;
        return jsonBrew;
    }

    void fromJson(const JSON::Value& jsonBrew) {
        brewID = jsonBrew["brewID"].asInt();
        brewName = jsonBrew["brewName"].asString();
        brewType = jsonBrew["brewType"].asString();
        brewDate = jsonBrew["brewDate"].asString();
        brewStatus = jsonBrew["brewStatus"].asString();
        IG = jsonBrew["IG"].asDouble();
        FG = jsonBrew["FG"].asDouble();
        volume = jsonBrew["volume"].asDouble();
        ABV = jsonBrew["ABV"].asDouble();
    }


    // Setters and getters for brew data
    void setBrewID(int id) {
        brewID = id;
    }

    void setIG(double ig) {
        IG = ig;
    }

    void setFG(double fg) {
        FG = fg;
    }

    void setABV(double abv) {
        ABV = 131.5*(IG-FG);
    }

    void setVolume(double vol) {
        volume = vol;
    }

 void setBrewName(const std::string& name) {
        brewName = name;
    }

    void setBrewType(const std::string& type) {
        brewType = type;
    }

    void setBrewDate(const std::string& date) {
        brewDate = date;
    }

    void setBrewStatus(const std::string& status) {
        brewStatus = status;
    }


    int getBrewID() const {
        return brewID;
    }

    double getIG() const {
        return IG;
    }

    double getFG() const {
        return FG;
    }   

    double getABV() const {
        return ABV;
    }

    double getVolume() const {
        return volume;
    }
   
    std::string getBrewName() const {
        return brewName;
    }

    std::string getBrewType() const {
        return brewType;
    }

    std::string getBrewDate() const {
        return brewDate;
    }

    std::string getBrewStatus() const {
        return brewStatus;
    }

    

};


class BrewDatabase {
private:

    std::map<int, Brew> brews; // Map to store Brew objects

    std::string databaseFilename = "brewDatabase.json"; // Default log file name


public:
    BrewDatabase()  {
        if std::filesystem::exists(databaseFilename) {
            loadBrewDatabase();
        } else {
            BOOST_LOG_TRIVIAL(info) << "Brew database file not found, starting with an empty database.";
            std::ofstream outputFile(databaseFilename);
            outputFile << "{}"; // Create an empty JSON object
            outputFile.close();
        }
        

    } 

    void addBrew(const Brew& brew) {
        nBrews = brews.size();
        brew.setBrewID(nBrews + 1); // Assign a new ID
        brews[nBrews + 1] = brew;
    }

    Brew getBrew(int brewID) const {
        auto it = brews.find(brewID);
        if (it != brews.end()) {
            return it->second;
        }
        throw std::runtime_error("Brew not found");
    }

    void removeBrew(int brewID) {
        brews.erase(brewID);
    }


    
    // Serialize the map to JSON
    Json::Value toJson() const {
        JSON::Value root(JSON::objectValue);
        for (const auto& [id, brew] : brews) {
            root[std::to_string(id)] = brew.toJson();
        }
        return root;
    }

    // Deserialize the map from JSON
    void fromJson(const JSON::Value& root) {
        brews.clear();
        for (const auto& key : root.getMemberNames()) {
            Brew brew;
            brew.fromJson(root[key]);
            int id = brew.getBrewID();
            brews[id] = brew;
        }
    }


    // Save to file
    void saveBrewDatabase() const {
        std::ofstream file(databaseFilename);
        if (file.is_open()) {
            JSON::StreamWriterBuilder writer;
            file << JSON::writeString(writer, toJson());
            file.close();
        }
    }

    // Load from file
    void loadBrewDatabase() {
        std::ifstream file(databaseFilename);
        if (file.is_open()) {
            JSON::Value root;
            JSON::CharReaderBuilder reader;
            std::string errs;
            bool ok = JSON::parseFromStream(reader, file, &root, &errs);
            if (ok) fromJson(root);
            file.close();
        }
    }

        

};