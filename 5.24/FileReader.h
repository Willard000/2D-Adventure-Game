#include <string>
#include <unordered_map>

#ifndef FILE_READER_H
#define FILE_READER_H

bool file_exists(const char *path);

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
	bool get_bool(std::string key) { return (bool)std::stoi(_data[key]); }

	bool exists(std::string key) { return _data.find(key) != _data.end(); }

	freader::iterator begin() { return _data.begin(); }
	freader::iterator end() { return _data.end(); }
private:
	bool _is_read;
	std::string _path;
	freader::Data _data;
};

#endif