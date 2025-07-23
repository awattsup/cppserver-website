#include <cstdlib>
#include <string>
#include <cmath>



#include <map>
#include <memory>

// Base class for all devices
class Device {
protected:
    std::string deviceId;
    std::string statusfileName = deviceId + "_statusfile.json"; // Default state file name
public:
    virtual ~Device() = default;

    virtual void setDeviceId(const std::string& id) {
        deviceId = id;
    }    

    virtual std::string getDeviceId() const {
        return deviceId;
    }   

    virtual std::string getStatusFileName() const {
        return statusfileName;
    }

};



class iSpindle : public Device {
private:
    // Private iSpindle data
    double temperature;
    double gravity;
    double angle;
    double batteryVoltage;
    double calibCoeffs[4]; 
    double calibGravity;
    int assignedBrewId = -1; // Default value indicating no brew is assigned

public:
    iSpindle() = default; //TODO add constructor to initialize iSpindle data

    //TODO add JSON serialization methods

    // Add methods to handle iSpindle data
    void processData(const std::string& data) {
        // Process the incoming data
    }

    // Setters and getters for iSpindle data 

    void setTemperature(double temp) {
        temperature = temp;
    }

    void setGravity(double grav) {
        gravity = grav;
    }

    void setAngle(double ang) {
        angle = ang;
    }   

    void setBatteryVoltage(double voltage) {
        batteryVoltage = voltage;
    }
    
    void setCalibCoeffs(const double coeffs[4]) {
        for (int i = 0; i <= 3; i++) {
            calibCoeffs[i] = coeffs[i];
        }
    }

    void setCalibGravity(double calibGrav, const double calibCoeffs[4]) {
        calibGravity = 0; 
        for (int i = 0; i <= 3; i++) {
            calibGravity += calibCoeffs[i] * pow(angle, i);
        };
    }

    void setAssignedBrewId(int brewId) {
        assignedBrewId = brewId;
    }

    double getTemperature() const {
        return temperature;
    }

    double getGravity() const {
        return gravity;
    }

    double getAngle() const {
        return angle;
    }

    double getBatteryVoltage() const {
        return batteryVoltage;
    }

    double getCalibGravity() const {
        return calibGravity;
    }

    double getCalibCoeffs(int index) const {
        if (index < 0 || index >= 4) {
            throw std::out_of_range("Index out of range for calibration coefficients");
        }
        return calibCoeffs[index];
    }

    int getAssignedBrewId() const {
        return assignedBrewId;
    }


};




// DeviceList manages all devices (of any type)
class DeviceList {
private:
    std::map<std::string, std::unique_ptr<Device>> devices;

public:
    // Add a device (takes ownership)
    void addDevice(std::unique_ptr<Device> device) {
        devices[device->getDeviceId()] = std::move(device);
    }

    // Get a device by ID (nullptr if not found)
    Device* getDevice(const std::string& id) const {
        auto it = devices.find(id);
        if (it != devices.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    // Remove a device by ID
    void removeDevice(const std::string& id) {
        devices.erase(id);
    }

};

