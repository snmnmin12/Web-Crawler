
#include "sharedData.h"

Parameter::Parameter(SharedData* _shareData, int numthreads) : sharedData(_shareData) {
	sharedData->threadCount = numthreads;
} 

bool Parameter::checkHost(char* host) {

	std::unique_lock<std::mutex> lck(mutex_host);
	int preS = sharedData->seenHosts.size();
	sharedData->seenHosts.insert(host);	
	if (sharedData->seenHosts.size() == preS) {
		lck.unlock();
		return false;
	} 
	lck.unlock();
	return true;

}

bool Parameter::checkIps(in_addr_t Ip) {

	std::unique_lock<std::mutex> lck(mutex_IP);
	int preS = sharedData->seenIPs.size();
	sharedData->seenIPs.insert(Ip);
	if (sharedData->seenIPs.size() == preS){
		lck.unlock();
		return false;
	} 
	lck.unlock();
	return true;	
}

bool Parameter::queueEmpty() {
	std::unique_lock<std::mutex> lck(mutex_ep);
	bool res = (sharedData->pendingURLs.size() == 0);
	// lck.unlock();
	return res;
}
std::string Parameter::quePop() {
	std::unique_lock<std::mutex> lck(mutex_q);
	if (sharedData->pendingURLs.size() == 0) {
		lck.unlock();
		return "";
	}
	std::string s = sharedData->pendingURLs.front();
	sharedData->pendingURLs.pop();
	lck.unlock();
	return s;
}

void Parameter::DecreaseThread() {
	std::unique_lock<std::mutex> lck(mutex_thread);
	if (sharedData->threadCount > 0)
		sharedData->threadCount--;
	lck.unlock();
}

void Parameter::IncreaseExtracted() {
	std::unique_lock<std::mutex> lck(mutex_extracted);
		sharedData->extractedURLs++;
	lck.unlock();
}

void Parameter::IncreaseDNS() {
	std::unique_lock<std::mutex> lck(mutex_dns);
		sharedData->successfulDNS++;
	lck.unlock();
}

void Parameter::IncreaseRoboPass(){
	std::unique_lock<std::mutex> lck(mutex_robot);
	sharedData->urlsPassedRobots++;
	lck.unlock();
}

void Parameter::IncreaseCrawledURL() {
	std::unique_lock<std::mutex> lck(mutex_crawled);
	sharedData->crawledURLs++;
	lck.unlock();
}

void Parameter::IncreaseTotalLinks(unsigned long links) {
	std::unique_lock<std::mutex> lck(mutex_totalLinks);
		sharedData->totalLinks += links;
		// printf("total links %lu \n", sharedData->totalLinks);
	lck.unlock();
}

void Parameter::IncreaseTotalBytes(unsigned long bytes) {
	std::unique_lock<std::mutex> lck(mutex_totalBytes);
		sharedData->totalBytes  += bytes;
		// printf("total bytes %lu \n", sharedData->totalBytes);
	lck.unlock();
}

void Parameter::IncreaseCodes(unsigned int code) {
	unsigned int c = code / 100;
	std::unique_lock<std::mutex> lck;
	switch (c) {
		case 2:
			lck = std::unique_lock<std::mutex>(mutex_xx2);
			sharedData->xx2 += 1;
			lck.unlock();
			break;
		case 3:
			lck = std::unique_lock<std::mutex>(mutex_xx3);
			sharedData->xx3 += 1;
			lck.unlock();
			break;
		case 4:
			lck = std::unique_lock<std::mutex>(mutex_xx4);
			sharedData->xx4 += 1;
			lck.unlock();
			break;
		case 5:
			lck = std::unique_lock<std::mutex>(mutex_xx5);
			sharedData->xx5 += 1;
			lck.unlock();
			break;
		default:
			lck = std::unique_lock<std::mutex>(mutex_other);
			sharedData->other +=1;
			lck.unlock();
	}
}
