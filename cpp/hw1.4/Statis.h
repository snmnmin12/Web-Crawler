//Created by Mingmin Song on 1/28/18.
#pragma once
#include <unistd.h>
#include "sharedData.h"
#include <thread>

class Statistic {
public:
	Statistic(const SharedData* _shareData);
	void run();
	void shutdown();
private:
	std::thread *th = NULL;
	bool shutdownFlag = false;
	const SharedData* sharedData;
};