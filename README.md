
# cppserver-website


A lightweight C++ web server using the CROW framework. Designed for IoT data collection via HTTP/S with the server running on a small device like Raspberry Pi.

## Features
- Receives JSON data via HTTP POST at `/post_data`
- Stores received data in `data.json`


## Build Instructions

1. Open a terminal and navigate to the `cppserver-website` directory:
   ```sh
   cd cppserver-website
   ```
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

## Test usage
Send a POST request with JSON data to `http://<device-ip>:8080/post_data` (or the port you set with the `PORT` environment variable).

Example using curl:
```sh
curl -X POST -H "Content-Type: application/json" -d '{"temp":22.5}' http://localhost:8080/post_data
```

## Configuring your device
If your IoT device has configuration settings you can configure it to send the data to the sever with the following setup

Server address/URL: http://<device-ip> 
PORT: 8080 (or the port you have set the server to listen on)
API/Token: Leave blank, no API keys currently implemented
URI/Endpoint: post_data/ (or your folder name where the data will go)


