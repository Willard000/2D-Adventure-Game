#include <fstream>

#ifndef LOG_H
#define LOG_H

class Log {
public:
	Log();
	~Log();

	void print(std::string val, const char *end = "\n");

	template <class T>
	void print(const char *name, T val, const char *end = "\n") {
		_log << "               " << name << ": " << val << end;
	}
private:
	std::string system_time();
private:
	int _logNum;
	std::ofstream _log;
};

#endif
