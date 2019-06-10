#include "FileReader.h"

FileReader::FileReader(const char *path, bool isRead) {
	_path = path;
	if (!isRead) {
		_isRead = false;
		return;
	}

	read();
}

bool FileReader::read() {
	if (_isRead) {
		return true;
	}

	Clock timer(-1);

	printf("\nReading file: \"%s\".\n", _path.c_str());
	std::fstream file(_path.c_str());

	if (!file.is_open()) {
		printf("Couldn't open file: \"%s\".\n", _path.c_str());
		_isRead = false;
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

	timer.update();
	printf("File read in: %f second(s).\n\n", timer.getTime());

	file.close();

	_isRead = true;
	return true;
}