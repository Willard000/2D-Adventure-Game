#include <string>

#include <fstream>
//#include <cstdio>

#include <vector>
#include <unordered_map>

#include "Clock.h"

#ifndef FILE_READER_H
#define FILE_READER_H

#define MAX_TYPE_LENGTH 30
#define MAX_PATH_LENGTH 125

#define FILE_DELIMITER " "
#define FILE_COMMENT_CHAR '#'

/*
namespace freader {
	typedef std::pair<std::string, std::string> Line;
	typedef std::vector<Line> Data;
}

class FileReader {
public:
	FileReader(const char *path, bool isRead = true);

	bool read();

	freader::Line &line(unsigned int index)	         { return _data[index];					  }
	std::string first_string(unsigned int index)     { return _data[index].first;			  }
	std::string second_string(unsigned int index)    { return _data[index].second;			  }
	int first_int(unsigned int index)				 { return std::stoi(_data[index].first);  }
	int second_int(unsigned int index)			     { return std::stoi(_data[index].second); }
	double second_double(unsigned int index)		 { return std::stod(_data[index].second); }
	unsigned int size()							     { return _data.size();					  }
private:
	bool _isRead;
	std::string _path;
	freader::Data _data;
};

*/

namespace freader {
	typedef std::unordered_map<std::string, std::string> Data;
	typedef Data::iterator iterator;
}

class FileReader {
public:
	FileReader(const char *path, bool isRead = true);

	bool read();

	std::string get_string(std::string key) {return _data[key];}
	int get_int(std::string key) { return std::stoi(_data[key]); }
	double get_double(std::string key) { return std::stod(_data[key]); }

	bool exists(std::string key) { return _data.find(key) != _data.end(); }

	freader::iterator begin() { return _data.begin(); }
	freader::iterator end() { return _data.end(); }
private:
	bool _isRead;
	std::string _path;
	freader::Data _data;
};

#endif