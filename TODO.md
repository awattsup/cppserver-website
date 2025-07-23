# TODO

## Completed
- [x] Set up CROW C++ web server
- [x] Serve static HTML/CSS/JS from `public/`
- [x] Implement POST endpoint to receive JSON data
- [x] Store received data locally
- [x] Organize code into `src/` and `include/` directories
- [x] Implement base `Device` class and `iSpindle` derived class
- [x] Implement `DeviceList` to manage devices
- [x] Add basic getters/setters for device attributes

## In Progress
- [ ] Add JSON serialization/deserialization for devices
- [ ] Add endpoint to dynamically add devices from POST data

## Planned
- [ ] Implement authentication for POST endpoints
- [ ] Add web UI for managing devices and brews
- [ ] REST API for querying device and brew status
- [ ] Add persistent storage for device and brew databases
- [ ] Add unit tests for device and database logic
- [ ] Improve error handling and logging
- [ ] Refactor device class hierarchy for extensibility