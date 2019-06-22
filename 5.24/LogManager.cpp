#include "LogManager.h"

#include <string>

#include "Environment.h"
#include "Clock.h"

#define LOCATE_FILE_PATH "Data/Log/logNum.txt" 
#define LOG_PATH "Data/Log/"

bool file_exists(const char *path) {
	std::ifstream file(path);
	return file.good();
}

std::string LogManager::system_time() {
	return (Environment::get().getClock()->getDisplayTime() + " - ");
}

LogManager::LogManager() :
	_logNum			( 0 )
{
	std::string path = "";

	std::ifstream locate_file_i(LOCATE_FILE_PATH, std::ios::in);
	locate_file_i >> _logNum;
	locate_file_i.close();
	std::ofstream locate_file_o(LOCATE_FILE_PATH, std::ios::out | std::ios::trunc);
	locate_file_o << _logNum + 1;
	locate_file_o.close();

	path = LOG_PATH + std::string("log") + std::to_string(_logNum) + ".txt";
	_log.open(path.c_str());

	log("Starting log");
}

LogManager::~LogManager() {
	log("Closing log");
	_log.close();
}

void LogManager::log(std::string val, const char *end) {
	_log << system_time() << val << end;
}