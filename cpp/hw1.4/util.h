//Created by Mingmin Song on 1/28/18.

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unordered_set>
#include <string> 
#include <errno.h>
#include <vector>

#include "network.h"
#include "sharedData.h"

#define MAX_HOST_LEN		256
#define MAX_REQUEST_LEN		2048

std::string getRobot(const char* host);
std::string  getPage(const char *request, const char *host);
bool htmlProcessing(Socket &sockt, const char *host, const char *request, std::string& output, Parameter &param);
void loadURLs(const char * files, SharedData *sharedData);
bool preprocessing(const char *str, int *port, char *host, char *request, std::string& output);