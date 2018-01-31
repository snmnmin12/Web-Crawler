//  Created by Mingmin Song on 1/28/18.

#include "Statis.h"
#include "json.hpp"
#include <uWS/uWS.h>

using json = nlohmann::json;

Statistic :: Statistic (const SharedData *_shareData) : sharedData(_shareData){

	th = new std::thread(std::bind(&Statistic::run, this));
}


void Statistic :: run() {

	uWS::Hub h;

    h.onMessage([this](uWS::WebSocket<uWS::CLIENT> ws, char *message, size_t length, uWS::OpCode opCode) {

		printf("Received message\n");
		unsigned long activeThreads, Q, E, H, D, R, I, L, C;
		unsigned long lastCount = 0;
		unsigned long lastBytes = 0;
		unsigned long second = 0;
		double pps = 0;
		double mbps = 0;

	while(!shutdownFlag) {

		int sleep_time = 1;
		// sleep(2);
		json msgJson;
		activeThreads = sharedData->threadCount;
		Q = sharedData->pendingURLs.size();
		if (Q == 0) shutdownFlag = true;
		E = sharedData->extractedURLs;
		H = sharedData->seenHosts.size();
		D = sharedData->successfulDNS;
		I = sharedData->seenIPs.size();
		R = sharedData->urlsPassedRobots;
		C = sharedData->crawledURLs;
		L = sharedData->totalLinks;

		pps  = (second==0?0 : (double) (C - lastCount) / (double)second);
		mbps = (second==0?0 : (sharedData->totalBytes - lastBytes) / (double)(125000 * second));
	    lastCount = C;
		lastBytes = sharedData->totalBytes;

		msgJson["Q"] 	= Q;
		msgJson["E"] 	= E;
		msgJson["H"] 	= H;
		msgJson["D"]	= D;
		msgJson["I"] 	= I;
		msgJson["R"] 	= R;
		msgJson["C"] 	= C;
		msgJson["L"] 	= L;
		msgJson["pps"] 	= pps;
		msgJson["mbps"] = mbps;
		msgJson["time"] = second;
		msgJson["xx2"]  = sharedData->xx2;
		msgJson["xx3"]  = sharedData->xx3;
		msgJson["xx4"]  = sharedData->xx4;
		msgJson["other"]  = sharedData->other;

		std::string msg = msgJson.dump();
		ws.send(msg.data(), msg.length(), opCode);

		printf("[%3lu] %3lu Q%6lu E%7lu H%6lu D%6lu I%5lu R%5lu C%5lu L%4luK\n", second, activeThreads, Q, E, H, D, I, R, C, L / 1000);
		printf("\t*** crawling %.1f pps @ %.1f Mbps\n", pps, mbps);
		sleep(sleep_time);
		second += sleep_time;
	    }

	});

	h.onConnection([&h](uWS::WebSocket<uWS::CLIENT> ws, uWS::HttpRequest req) {
        std::cout << "Connected to Server!!!" << std::endl;
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::CLIENT> ws, int code,
        char *message, size_t length) {
        ws.close();
        std::cout << "Disconnected" << std::endl;
    });

    // h.connect("ws:://localhost:4567", nullptr); 
    h.connect("ws:://ec2-54-213-242-87.us-west-2.compute.amazonaws.com:4567", nullptr); 
    h.run();

	// printf("Extracted %lu URLS @ %i/s\n", E, (int)((double)E / (double)second));
	// printf("Looked up %lu DNS names @ %i/s\n", D, (int)((double)D / (double)second));
	// printf("Downloaded %lu robots @ %i/s\n", R, (int)((double)R / (double)second));
	// printf("Crawled %lu pages @ %i/s\n", C, (int)((double)C / (double)second));
	// printf("Parsed %lu links @ %i/s\n", L, (int)((double)L / (double)second));
	// printf("HTTP codes: 2xx = %lu, 3xx = %lu, 4xx = %lu, 5xx = %lu, other = %lu\n", sharedData->xx2, sharedData->xx3, sharedData->xx4, sharedData->xx5, sharedData->other);

}

void Statistic :: shutdown() {
	shutdownFlag = true;
	if (th != NULL) {
		th->join();
	}
	delete th;
}