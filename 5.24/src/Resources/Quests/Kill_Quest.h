#include "Quest.h"

#ifndef KILL_QUEST_H
#define KILL_QUEST_H 

class Kill_Quest : public Quest {
public:
	Kill_Quest(int id, int target_id, int total) :
		Quest			( id ),
		_target_id		( target_id ),
		_count			( 0 ),
		_total			( total )
	{}

	int get_type() { return 0; }

	void save(std::ofstream &file) {
		file << FILE_QUEST_ID << " " << _id << " "
			 << FILE_QUEST_ACTIVE << " " << _active << " "
			 << FILE_QUEST_COMPLETE << " " << _complete << " "
			 << FILE_QUEST_COUNT << " " << _count << " ";
	}

	void load(std::stringstream stream) {
		std::string key, data;
		while (stream >> key) {
			stream >> data;
			if (key == FILE_QUEST_ID) _id = std::stoi(data);
			else if (key == FILE_QUEST_ACTIVE) _active = (bool)std::stoi(data);
			else if (key == FILE_QUEST_COMPLETE) _complete = (bool)std::stoi(data);
			else if (key == FILE_QUEST_COUNT) _count = std::stoi(data);
		}
	}

protected:
	int _target_id;
	int _count;
	int _total;
};

#endif