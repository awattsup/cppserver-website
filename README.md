
# cppserver-website


A lightweight C++ web server using the CROW framework. Designed for IoT data collection on devices like Raspberry Pi. Now located in the `spacebrews-cppserver` subdirectory.

## Features
- Receives JSON data via HTTP POST at `/post_data`
- Stores received data in `data.json`


## Build Instructions

1. Open a terminal and navigate to the `spacebrews-cppserver` directory:
   ```sh
   cd spacebrews-cppserver
   ```
2. Install CMake and a C++17 compiler (e.g., g++ or clang++)
3. Build the project:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
4. Run the server:
   ```sh
   ./cppserver
   ```

## Usage
Send a POST request with JSON data to `http://<device-ip>:8080/post_data` (or the port you set with the `PORT` environment variable).

Example using curl:
```sh
curl -X POST -H "Content-Type: application/json" -d '{"temp":22.5}' http://localhost:8080/post_data
```
