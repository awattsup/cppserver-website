# TODO

## In Progress

- [ ] Refactor logging so each device assigned to a brew writes to a unique log file (include deviceID or deviceName in filename)
- [ ] Decouple device and database logging logic: device provides data, handler/database constructs log filename and writes log
- [ ] Implement device method for returning log data.
- [ ] Add web UI for managing devices  
- [ ] Add device type attribute
- [ ] Move time-date function to devices

## Planned
- [ ] Implement authentication for POST endpoints
- [ ] REST API for querying device and databse status
- [ ] Add persistent storage for device and  databases
- [ ] Add unit tests for device and database logic
- [ ] Improve error handling and logging
- [ ] Populate website side of the codebase

## Completed
- [x] Implement update of data logs by devices when POST data is received. 
- [x] Add endpoint to dynamically update/add new devices from POST data
- [x] Refactor device class hierarchy for extensibility
- [x] Add constructor for loading a device from its status file
- [x] Add constructor for making a new device given POST data
- [x] Modified CROW post route to convert device string to JSON and prototyped processing into separate functions. 
- [x] Verified data receiving on HTTP POST on local network
- [x] Add JSON serialization/deserialization for devices
- [x] Add basic getters/setters for device attributes
- [x] Implement `DeviceList` to manage devices
- [x] Serve static HTML/CSS/JS from `public/`
- [x] Implement base `Device` class and `iSpindle` derived class
- [x] Organize code into `src/` and `include/` directories
- [x] Store received data locally
- [x] Implement POST endpoint to receive JSON data
- [x] Set up CROW C++ web server



