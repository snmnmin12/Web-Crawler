
#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/select.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <unistd.h>

#define THRESHOLD 10
#define KB * 1024
#define INITIAL_SIZE 8 KB
#define requestLength 3 KB

class Socket{
public:
	enum Type{
		robot,
		page
	};
	Socket(struct sockaddr_in&);
	~Socket();
	int Read(Type type);
	bool Connect(std::string& req);
	// bool ConnectContent(char * request);
	bool SendRequest(std::string& req);
	char *getBuffer() {return buffer;};
private:
	int sock;
	char *buffer;
	int allocateSize;
	int curPos;
	int ret;
	unsigned maxDownableSize;
	struct sockaddr_in serv_addr;
};