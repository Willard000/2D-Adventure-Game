#include <fstream>

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

class LogManager {
public:
	LogManager();
	~LogManager();

	
	void log(std::string val, const char *end = "\n");

	template <class T>
	void log(const char *name, T val, const char *end = "\n") {
		_log << "               " << name << ": " << val << end;
	}
private:
	std::string system_time();
private:
	const char *_LOG_PATH = "Data/Log/";

	int _logNum;
	std::ofstream _log;
};

#endif
