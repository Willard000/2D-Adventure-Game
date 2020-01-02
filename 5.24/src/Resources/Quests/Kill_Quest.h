#include "Quest.h"

#ifndef KILL_QUEST_H
#define KILL_QUEST_H 

class Kill_Quest : public Quest {
public:
	Kill_Quest(int id, int target_id, int total);

	int get_type();

	bool is_active();
	bool should_complete();
	bool is_complete();

	void activate();
	void complete();

	void save(std::ofstream &file);
	void load(std::stringstream stream);

	int get_kill_count();
	void add_kill();

	std::string get_info_string();

protected:
	int _target_id;
	int _count;
	int _total;
};

#endif