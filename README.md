# cppserver

A lightweight C++ web server using the CROW framework. Designed for IoT data collection on devices like Raspberry Pi.

## Features
- Receives JSON data via HTTP POST at `/post_data`
- Stores received data in `data.json`

## Build Instructions

1. Install CMake and a C++17 compiler (e.g., g++ or clang++)
2. Build the project:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
3. Run the server:
   ```sh
   ./cppserver
   ```

## Usage
Send a POST request with JSON data to `http://<device-ip>:18080/post_data`.

Example using curl:
```sh
curl -X POST -H "Content-Type: application/json" -d '{"temp":22.5}' http://localhost:18080/post_data
```
