#include "crow_all.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/filesystem.hpp>


void sendFile(crow::response& res, std::string filename, std::string contentType){
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
    sendFile(res,filename + ".html","text/html");
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

    CROW_ROUTE(app, "/post_data").methods(crow::HTTPMethod::Post)([](const crow::request& req){
        auto body = req.body;
        std::ofstream ofs("data.json", std::ios::app);
        ofs << body << std::endl;
        return crow::response(200, "Data received");
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

    CROW_ROUTE(app, "/about")(
        [](const crow::request& req, crow::response& res){
        sendHtml(res, "about");            
    });

    CROW_ROUTE(app, "/")(
        [](const crow::request& req, crow::response& res){
        sendHtml(res, "index");            
    });

    char *port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL? std::stoi(port) : 8080);
    std::cout << "PORT = " << iPort << std::endl;
    app.port(iPort).multithreaded().run();

}
