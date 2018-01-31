
#include "network.h"
#include "util.h"
#include "sharedData.h"
#include "Statis.h"
#include <vector> 
#include <thread>

using namespace std;

void crawl(Parameter& param);

int main (int argc, char const *argv[]) {

	int num_thread = 1;
	const char *input;

	if (argc == 2) {
		input = argv[1];
	} else if (argc == 3) {
		num_thread = atoi(argv[1]);
		input = argv[2];
	} else {
		printf("Usage: execution num_thread input\n");
		exit(EXIT_FAILURE);
	}

	//use to handle duplicate host or IPs;
	SharedData* sharedData = new SharedData();
	// UniqueCheck unicheck;
    loadURLs(input, sharedData);

    Statistic sta(sharedData);
	Parameter param(sharedData, num_thread);

    printf("shared queue size: %lu\n", sharedData->pendingURLs.size());

	std::vector<std::thread> ths;

	for (int i = 0; i < num_thread; i++) {
		ths.push_back(std::thread(crawl, std::ref(param)));
	}

	for (int i = 0; i < num_thread; i++) {
		ths[i].join();
	}

	sta.shutdown();	
	printf("program finish successfully!\n");
	delete sharedData;
	return 0;
}

void crawl(Parameter& param) {

		while (true) {
			if (param.queueEmpty()) { 
				param.DecreaseThread();
				break;
			}
			std::string url = param.quePop();
			if (url == "") {
				param.DecreaseThread();
				break;
			}
			param.IncreaseExtracted();
			// printf("%s\n",url.c_str());
			const char * str = url.c_str();
			int port = 80;
			char host[MAX_HOST_LEN]={0};
			char request[MAX_REQUEST_LEN]={0};

			std::string output = url + "\n";
			if (!preprocessing(str, &port, host, request, output)) {
				// printf("%s", output.c_str());
				continue;
			}

			if(!param.checkHost(host)) {
				// printf("%s", output.c_str());
				continue;
			}
			in_addr_t IP = inet_addr(host);
			struct sockaddr_in serv_addr;
			struct hostent *remote; 
			if (IP == INADDR_NONE)
			{
				if ((remote = gethostbyname (host)) == NULL) {
					// printf("%s", output.c_str());
					continue;
				}
				else {
					memcpy ((char *)&(serv_addr.sin_addr), remote->h_addr, remote->h_length);
					param.IncreaseDNS();
				}
			} else {
				serv_addr.sin_addr.s_addr = IP;
			}
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(port);	

			if(!param.checkIps(serv_addr.sin_addr.s_addr)) {
				// printf("%s", output.c_str());
				continue;
			}
			Socket sockt(serv_addr);
			htmlProcessing(sockt, host, request, output, param);
			// printf("%s", output.c_str());
			// usleep(100);
		}
}