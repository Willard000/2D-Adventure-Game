#include "FileReader.h"

#include <fstream>

#define MAX_TYPE_LENGTH 30
#define MAX_PATH_LENGTH 125

#define FILE_DELIMITER " "
#define FILE_COMMENT_CHAR '#'

bool file_exists(const char *path) {
	std::ifstream file(path);
	return file.good();
}

FileReader::FileReader(const char *path, bool isRead) :
	_path		(path)
{
	if (!isRead) {
		_is_read = false;
		return;
	}

	read();
}

bool FileReader::read() {
	if (_is_read) {
		return true;
	}

	//printf("\nReading file: \"%s\".\n", _path.c_str());
	std::fstream file(_path.c_str());

	if (!file.is_open()) {
		printf("Couldn't open file: \"%s\".\n", _path.c_str());
		_is_read = false;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.size() > 1 && line[0] != FILE_COMMENT_CHAR) {
			std::string key, data;
			size_t split = line.find(FILE_DELIMITER);
			key = line.substr(0, split);
			data = line.substr(split + 1, line.size());
			_data[key] = data;
		}
	}

	file.close();

	_is_read = true;
	return true;
}