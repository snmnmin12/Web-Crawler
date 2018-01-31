//Created by Mingmin Song on 1/28/18.

#pragma once
#include <unordered_set>
#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <netinet/in.h>

class SharedData {
public:
	std::queue<std::string> pendingURLs;
	std::unordered_set<std::string> seenHosts;
	std::unordered_set<in_addr_t> seenIPs;
	unsigned long threadCount;
	unsigned long extractedURLs;
	unsigned long successfulDNS;
	unsigned long urlsPassedRobots;
	unsigned long urlsPassedIP;
	unsigned long crawledURLs;
	unsigned long totalLinks;
	unsigned long totalBytes;
	unsigned long xx2, xx3, xx4, xx5, other;

	SharedData() {
		threadCount      = 0;
		extractedURLs    = 0;
		successfulDNS    = 0;
		urlsPassedRobots = 0;
		crawledURLs      = 0;
		totalLinks       = 0;
		totalBytes       = 0;
		xx2 			 = 0;
		xx3 			 = 0;
		xx4 			 = 0;
		xx5 			 = 0;
		other 		     = 0;
	}
};

class Parameter {

public:
	SharedData* sharedData;
	std::mutex mutex_host, mutex_IP, mutex_dns, mutex_robot, mutex_crawled, mutex_links,
		mutex_extracted, mutex_totalLinks, mutex_q, mutex_totalBytes, mutex_xx2,
		mutex_xx3, mutex_xx4, mutex_xx5, mutex_other, mutex_thread, mutex_ep;

	Parameter(SharedData* _shareData, int numthreads);
	bool checkHost(char* host);
	bool checkIps(in_addr_t Ip);
	bool queueEmpty();
	std::string quePop();

	//modify the shared data
	void DecreaseThread();
	void IncreaseExtracted();
	void IncreaseDNS();
	void IncreaseRoboPass();
	void IncreaseCrawledURL();
	void IncreaseTotalLinks(unsigned long links);
	void IncreaseTotalBytes(unsigned long bytes);
	void IncreaseCodes(unsigned int code);
};

