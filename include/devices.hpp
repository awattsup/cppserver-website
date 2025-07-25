#pragma once
#include <string>
#include <map>
#include <memory>
#include <json/json.h>


class Device {
protected:
    std::string deviceID;
    std::string statusfileName = deviceID + "_status.json"; // Default status file name
public:
    Device();
    virtual ~Device();

    virtual void setDeviceId(const std::string& id);
    virtual std::string getDeviceId() const;
    virtual std::string getStatusFileName() const;

    // // JSON serialization
    // virtual Json::Value toJson() const = 0;
    // virtual void fromJson(const Json::Value& val) = 0;
};

class iSpindle : public Device {
private:
    double temperature;
    double gravity;
    double angle;
    double batteryVoltage;
    double calibCoeffs[4];
    double calibGravity;
    int assignedBrewId;
public:
    iSpindle();

    void processData(const std::string& data);

    // Setters
    void setTemperature(double temp);
    void setGravity(double grav);
    void setAngle(double ang);
    void setBatteryVoltage(double voltage);
    void setCalibCoeffs(const double coeffs[4]);
    void setCalibGravity(double calibGrav, const double calibCoeffs[4]);
    void setAssignedBrewId(int brewId);

    // Getters
    double getTemperature() const;
    double getGravity() const;
    double getAngle() const;
    double getBatteryVoltage() const;
    double getCalibGravity() const;
    double getCalibCoeffs(int index) const;
    int getAssignedBrewId() const;

    // JSON serialization
    Json::Value toDeviceJSON() const ;
    void fromDeviceJSON(const Json::Value& val) ;

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