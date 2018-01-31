//Created by Mingmin Song on 1/28/18.

#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <fcntl.h>

#include <time.h>

Socket:: Socket(struct sockaddr_in & _serv_addr) : serv_addr(_serv_addr) {

	//buffer = (char*) malloc(INITIAL_SIZE * sizeof(char));
	allocateSize  = INITIAL_SIZE;
	buffer = NULL;
}

Socket::~Socket() {
	if (buffer) free(buffer);
	shutdown(sock, 0);
}

bool Socket:: Connect(std::string& request) {

	if (allocateSize > 32 KB) {
		allocateSize = 8 KB; 
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		// printf("\n Socket creation error \n");
		return false;
	}	

	if ((ret=connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
			return false;
	}
	if (!SendRequest(request)) {
    	return false;
    }
	return true;
}

bool Socket::SendRequest(std::string &request) {


	ret = send(sock, request.c_str(), request.size(), 0);
	if (ret == -1) {
		// printf("failed in send request with %d\n", (errno));
		return false;
	}
	return true;
}

int Socket::Read(Type type) {

	maxDownableSize = 2 * 1000 KB;
	if (type == Socket::robot) {
		maxDownableSize = 16 KB;
	}

	ret = 0;
	curPos = 0;
	buffer = (char*) malloc(allocateSize * sizeof(char));
	memset(buffer, 0, sizeof(char)*allocateSize);

	struct pollfd pfd_read;
	int timeout = 10000;

	while (true) {

		// printf("fd is %d\n", sock);
		pfd_read.fd = sock;
		pfd_read.events = POLLIN;

		// if ((ret = select(sock + 1, &read_fd_set, NULL, NULL, &timeout)) > 0) {
		if ((ret = poll(&pfd_read, 1, timeout)) > 0) {

				int bytes = recv(sock, buffer+curPos, allocateSize - curPos - 1, 0);

				curPos += bytes;

				if (curPos >= maxDownableSize) {
					buffer[curPos] = '\0';
					return 0;
				}

				if (bytes == 0) {
					buffer[curPos] = '\0';
					return 1;

				} else if (bytes == -1) {
					// printf("failed with %d on recv\n", errno);
					break;
				}

			   if (curPos + THRESHOLD > allocateSize) {
					allocateSize *= 2;
					char *n_buffer =  (char*)malloc(sizeof(char)*allocateSize);
					memcpy(n_buffer, buffer, curPos);
					free(buffer);
					buffer = n_buffer;
				}
		} else if (ret == 0) {
			break;
		} else  {
			break;
		}
	}
	return -1;
}