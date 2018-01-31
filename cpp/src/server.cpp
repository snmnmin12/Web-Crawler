#include <fstream>
#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>
#include "json.hpp"

int main()
{
    /* this is an echo server that properly passes every supported Autobahn test */
    uWS::Hub h;
    // Socket h;
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> ws, char *message, size_t length, uWS::OpCode opCode) {

        std::cout << "Received from server" << std::endl;
        while(true) {
            sleep(2);
             ws.send(message, length, opCode);
        }
        // ws.send(message, length, opCode);
    });

    h.onConnection([&h](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        std::cout << "Connected!!!" << std::endl;
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::CLIENT> ws, int code,
        char *message, size_t length) {
        ws.close();
        std::cout << "Disconnected" << std::endl;
    });

    h.connect("http:://localhost:4567", nullptr); 
    h.run();
}