#include "LogManager.h"

#include <string>

#include "Environment.h"
#include "Clock.h"

bool file_exists(const char *path) {
	std::ifstream file(path);
	return file.good();
}

std::string LogManager::system_time() {
	return (Environment::get().getClock()->getDisplayTime() + " - ");
}

LogManager::LogManager() {
	int logNum = 0;
	std::string path = "";
	do {
		logNum++;
		path = _LOG_PATH + std::string("log") + std::to_string(logNum) + ".txt";
	} while (file_exists(path.c_str()));
	_log.open(path.c_str());

	_logNum = logNum;

	log("Starting log");
}

LogManager::~LogManager() {
	log("Closing log");
	_log.close();
}

void LogManager::log(std::string val, const char *end) {
	_log << system_time() << val << end;
}