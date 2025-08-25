#include "crow_all.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/filesystem.hpp>

#include "database.hpp"
#include "devices.hpp"
#include "httpdatahandler.hpp"

#include <json/json.h> // JsonCpp library header



void sendFile(crow::response& res, std::string filename, std::string contentType){
    std::cout << "Sending file: " << filename << std::endl;
    std::cout << "Sending file: " << "./public/" +filename << std::endl;

    std::ifstream in("./public/" + filename, std::ifstream::in);
    if (in){
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", contentType);
        res.write(contents.str());
    }
    else {
        res.code = 404;
        res.write("Not found");
    }
    res.end();
}

void sendHtml(crow::response& res, std::string filename){
    sendFile(res,filename,"text/html");
}

void sendImage(crow::response& res, std::string filename){
    sendFile(res,"images/" + filename,"image/jpeg");
}

void sendScript(crow::response& res, std::string filename){
    sendFile(res,"scripts/" + filename,"text/javascript");
}

void sendStyle(crow::response& res, std::string filename){
    sendFile(res,"styles/" + filename,"text/css");
}


int main()
{
    crow::SimpleApp app;

    //innitialize the database and devicelist
    BrewDatabase brewDatabase;
    DeviceList deviceList;


    // CROW_ROUTE(app, "/post_data").methods(crow::HTTPMethod::Post)(
    //     [&deviceList](const crow::request& req){
    //         try {
    //             Json::Value test = req.body;
    //             processPOSTData(test, deviceList);
    //             // std::ofstream ofs("data.json", std::ios::app);
    //             // ofs << body << std::endl;
    //             return crow::response(200, "Data received");
    //         } catch (const std::exception& e) {
    //             // Log error
    //             std::cerr << "Error: " << e.what() << std::endl;
    //             return crow::response(500, "Server error");
    //         }
    //     }
    // );



    CROW_ROUTE(app, "/post_data").methods(crow::HTTPMethod::Post)([&deviceList, &brewDatabase](const crow::request& req) {
        try {   
            Json::Reader reader;
            Json::Value postJSON;
            bool ok = reader.parse(req.body, postJSON);
            if (ok) processPOSTData(postJSON, deviceList, brewDatabase);
            return crow::response(200, "Data received");
            } catch (const std::exception& e) {
                // Log error
                std::cerr << "Error: " << e.what() << std::endl;
                return crow::response(500, "Server error");
            }
    });

    CROW_ROUTE(app, "/styles/<string>")(
        [](const crow::request& req, crow::response& res, std::string filename){
        sendStyle(res, filename);            
    });

    CROW_ROUTE(app, "/scripts/<string>")(
        [](const crow::request& req, crow::response& res, std::string filename){
        sendScript(res, filename);            
    });

    CROW_ROUTE(app, "/images/<string>")(
        [](const crow::request& req, crow::response& res, std::string filename){
        sendImage(res, filename);            
    });

    CROW_ROUTE(app, "/blog/<path>")(
        [](const crow::request& req, crow::response& res, std::string path){
        sendHtml(res, "blog/" + path);           
    });

    CROW_ROUTE(app, "/about/<path>")(
        [](const crow::request& req, crow::response& res, std::string path){
        sendHtml(res, "about/" + path);            
    });

    CROW_ROUTE(app, "/")(
        [](const crow::request& req, crow::response& res){
        sendHtml(res, "index.html");            
    });

    // Serve internal UI
    CROW_ROUTE(app, "/internal")([](const crow::request&, crow::response& res){
        sendFile(res, "internal/index.html", "text/html");
    });

    CROW_ROUTE(app, "/internal/<string>")([](const crow::request&, crow::response& res, std::string filename){
        if (filename.size() > 3 && filename.substr(filename.size()-3) == ".js")
            sendFile(res, "internal/" + filename, "application/javascript");
        else if (filename.size() > 4 && filename.substr(filename.size()-4) == ".css")
            sendFile(res, "internal/" + filename, "text/css");
        else res.code = 404;
        res.end();
    });

    // REST API endpoints for brews
    CROW_ROUTE(app, "/api/brews").methods(crow::HTTPMethod::Get)
    ([&brewDatabase](const crow::request&){
        return crow::response(brewDatabase.toDatabaseJSON().toStyledString());
    });

    CROW_ROUTE(app, "/api/brews").methods(crow::HTTPMethod::Post)
    ([&brewDatabase](const crow::request& req){
        Json::Value brewJson;
        Json::Reader reader;
        if (!reader.parse(req.body, brewJson)) return crow::response(400, "Invalid JSON");
        Brew newBrew;
        newBrew.fromBrewJSON(brewJson);
        brewDatabase.addBrew(newBrew);
        return crow::response(201, "Brew created");
    });

    // REST API endpoints for devices
    CROW_ROUTE(app, "/api/devices").methods(crow::HTTPMethod::Get)
    ([&deviceList](const crow::request&){
        return crow::response(deviceList.toJson().toStyledString());
    });

    CROW_ROUTE(app, "/api/devices/<int>").methods(crow::HTTPMethod::Put)
    ([&deviceList](const crow::request& req, int id){
        Json::Value devJson;
        Json::Reader reader;
        if (!reader.parse(req.body, devJson)) return crow::response(400, "Invalid JSON");
        Device* dev = deviceList.getDevice(id);
        if (!dev) return crow::response(404, "Device not found");
        dev->updateDataFromPOST(devJson);
        return crow::response(200, "Device updated");
    });

    char *port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL? std::stoi(port) : 8080);
    std::cout << "PORT = " << iPort << std::endl;
    app.port(iPort).multithreaded().run();

}
