#pragma once
#include <string>
#include <map>
#include <memory>
#include <json/json.h>


class Device {
protected:
    std::string deviceName;
    int deviceID;
    std::string statusfileName = "data/devices/" + std::to_string(deviceID) + "_" + deviceName + "_status.json"; // Default status file name
    int RSSI; // Received Signal Strength Indicator
    std::string deviceType; // Type of device, e.g. iSpindle. 
public:
    Device();
    virtual ~Device();

    virtual std::string getTimeDate() const;

    virtual void setDeviceName(const std::string& name);
    virtual void setDeviceID(const int& id);

    virtual std::string getDeviceName() const;
    virtual int getDeviceID() const;
    virtual std::string getStatusFileName() const;
};

class iSpindle : public Device {
private:
    double temperature;
    double gravity;
    double angle;
    double batteryVoltage;
    double calibCoeffs[4];
    double calibGravity;
    int assignedBrewID = -1; // Default to -1 if not assigned
public:
    iSpindle();
    iSpindle(std::string statusfilePath);
    iSpindle(Json::Value postJSON);

    // JSON serialization
    Json::Value toStatusfileJSON() const ;
    void fromStatusfileJSON(const Json::Value& val) ;

    //file IO
    void saveStatusfile() const;
    void loadStatusfile();

    void updateDataFromPOST(const Json::Value& postJSON);

    std::string getLogData() const;

    // Setters
    void setTemperature(double temp);
    void setGravity(double grav);
    void setAngle(double ang);
    void setBatteryVoltage(double voltage);
    void setCalibCoeffs(const double coeffs[4]);
    void computeCalibGravity();
    void setRSSI(int rssi);
    void setAssignedBrewID(int brewId);

    // Getters
    double getTemperature() const;
    double getGravity() const;
    double getAngle() const;
    double getBatteryVoltage() const;
    double getCalibGravity() const;
    double getCalibCoeffs(int index) const;
    int getRSSI() const;
    int getAssignedBrewID() const;



};

class DeviceList {
private:
    std::map<std::string, std::unique_ptr<Device>> devices;
public:
    DeviceList();

    void addDevice(std::unique_ptr<Device> device);
    Device* getDevice(const std::string& id) const;
    void removeDevice(const std::string& id);
    size_t size() const;

    // // JSON serialization
    // Json::Value toDeviceJSON() const;
    // void fromDeviceJSON(const Json::Value& root);


};