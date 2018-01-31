#include <vector>
#include <fstream>
#include <set>
#include "util.h"
#include "urlExtraction.h"

std::string getRobot(const char* host) {
	std::string host_s(host);
	std::string output = "HEAD /robots.txt HTTP/1.0\r\nUser-agent: myTAMUcrawler/1.3\r\nHost: "+host_s+"\r\nConnection:close\r\n\r\n";
	return output;
}

std::string getPage(const char *request, const char *host) {
	std::string req_s(request);
	std::string host_s(host);
	std::string output = "GET "+req_s+" HTTP/1.0\r\nUser-agent: myTAMUcrawler/1.3\r\nHost: "+host_s+"\r\nConnection:close\r\n\r\n";
	return output;
}

void loadURLs(const char * files, SharedData *sharedData) {

	std::ifstream myfile(files);
	if (!myfile.is_open()) {
		printf("Unable to open files\n");
		exit(EXIT_FAILURE);
	} else {
		std::string line;
		unsigned long total = 0;
		while (getline(myfile, line)) {
			if (line[line.length()-1]=='\r') line.erase(line.length()-1);
			sharedData->pendingURLs.push(line);
			total += line.size();
		}
		printf("Opened %s with size %lu\n", files, total);
		myfile.close();
	}
}

bool preprocessing(const char *st, int *port, char *host, char *request, std::string& output) {
	char *str = (char*) st;
	char http[] = "http://";
	char *findhttp = strstr(str, http);
	if (findhttp == 0) {
		// printf("failed with invalid shceme\n");
		return false;
	}

	char *findFirstComma = strstr(str+strlen(http), ":");
	char *findFirstSlash = strstr(str+strlen(http), "/");
	char *findFirstMark  = strstr(str+strlen(http), "?");

	int lenPort = 0;
	if (findFirstComma) {
		if (findFirstSlash && (findFirstSlash - findFirstComma) > 0) {
			*port = atoi(findFirstComma + 1);
			int len = findFirstComma - (findhttp + strlen(http));
			if (len > MAX_HOST_LEN) return false;
			memcpy(host, findhttp+strlen(http), len);
			lenPort = 1 + (int)log10(*port) + 1;
			if (*port <=0 || *port >= 65536) {
				// printf("failed with invalid port\n");
				return false;
			}
		}
	}

	// char *findFirstSlash = strstr(str+strlen(http), "/");
	// char *findFirstMark  = strstr(str+strlen(http), "?");

	if (findFirstMark && (findFirstMark - findFirstSlash) < 0) findFirstSlash = 0;

	if (findFirstSlash) {
		char *findHash = strstr(findFirstSlash, "#");
		if (findHash) *findHash = '\0';
		int lenhost = findFirstSlash - lenPort - (findhttp + strlen(http));
		if (lenhost > MAX_HOST_LEN) return false;
		memcpy(host, findhttp+strlen(http), lenhost);
		int lenreq = str + strlen(str) - findFirstSlash;
		if (lenreq > MAX_REQUEST_LEN) return false;
		memcpy(request, findFirstSlash, lenreq);

	} else {
		if (findFirstMark) {
			int lenhost = findFirstMark - (findhttp + strlen(http)) - lenPort;
			if (lenhost > MAX_HOST_LEN) return false;
			memcpy(host, findhttp+strlen(http), lenhost);
			int lenreq = str + strlen(str) - findFirstMark;
			request[0]='/';
			if (lenreq > MAX_REQUEST_LEN) return false;
			memcpy(request + 1, findFirstMark, lenreq);
		} else {
			request[0]='/';
			int len = strlen(str) - strlen(http) - lenPort;
			if (len > MAX_HOST_LEN) return false;
			memcpy(host, findhttp+strlen(http), len);
		}
	}
	
	// output = output + "host " + std::string(host) + ", port " + std::to_string(*port) + ", request " + std::string(request) + "\r\n";
	// printf("host %s, ", host);
	// printf("port %d, ", *port);
	// printf("request %s\n", request);
	return true;
}


bool htmlProcessing(Socket &sockt, const char *host, const char *request, std::string& output, Parameter &param) {

		std::string robReq = getRobot(host);
		if (!sockt.Connect(robReq)) {
			return false;
		}
		int ret = sockt.Read(Socket::robot);
		if (ret == -1) {
			return false;
		}
		char* data = sockt.getBuffer();
		int status_code = atoi(data + 9);
		param.IncreaseTotalBytes((unsigned long)strlen(data));
		param.IncreaseCodes((unsigned int)status_code);
		//maximum downloadable size
		if (ret == 0) return false;
		// // std::string output = "";
		// // output = output + "\tVerifying header... status code " + std::to_string(status_code) + "\r\n"; 
		// // printf("\tVerifying header... status code %d\n", status_code);

	 //  	//in case robot does not exists
	  	if ((status_code / 100) == 4) {
	  		param.IncreaseRoboPass();
	  		// char requests[1024];
	  		// sprintf(requests, "GET %s HTTP/1.0\r\nUser-agent: myTAMUcrawler/1.2\r\nHost: %s\r\nConnection:close\r\n\r\n", request, host);
	  		std::string pageReq = getPage(request, host);
	  		// output += std::string(pageReq);
		  	if (!sockt.Connect(pageReq)) {
		  		return false;
		  	}

			if (!sockt.Read(Socket::page)) {
				return false;
			}
			data = sockt.getBuffer();
			param.IncreaseTotalBytes((int)strlen(data));

			status_code = atoi(data + 9);
			param.IncreaseCodes((unsigned int)status_code);

			// output += "\tVerifying header... status code " + std::to_string(status_code) + "\r\n";
			// printf("\tVerifying header... status code %d\n", status_code);

			if ((status_code / 100) == 2) {
				// output += "      + Parsing page...";
				// printf("      + Parsing page...");
				// clock_t begin = clock();
				std::string text(data);
				std::string url(host);
				url = "http://" + url;
				std::set<std::string> res = extract_hyperlinks(text, url);
				param.IncreaseCrawledURL();
				param.IncreaseTotalLinks((unsigned long)res.size());
				// clock_t end = clock();
				// int time = (int)(1000.0f*(double)(end - begin)/CLOCKS_PER_SEC);
				// output += "done in "+std::to_string(time) + " ms with "+ std::to_string(res.size())+" links\r\n";
			} 
	  	} 
	 	return true;
}