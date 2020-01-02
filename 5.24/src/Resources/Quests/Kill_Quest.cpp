#include "Kill_Quest.h"

#include "QuestLoader.h"

Kill_Quest::Kill_Quest(int id, int target_id, int total) :
	Quest(id),
	_target_id(target_id),
	_count(0),
	_total(total)
{}

int Kill_Quest::get_type() { 
	return 0;
}

bool Kill_Quest::is_active() {
	return _active;
}

bool Kill_Quest::should_complete() {
	return _count >= _total;
}

bool Kill_Quest::is_complete() {
	return _complete;
}

void Kill_Quest::activate() {
	_active = true;
}

int Kill_Quest::get_kill_count() {
	return _count;
}

void Kill_Quest::complete() {
	_complete = true;
}

void Kill_Quest::add_kill() {
	++_count;
}

std::string Kill_Quest::get_info_string() {
	std::string info = "You have killed " + std::to_string(_count) + " out of " + std::to_string(_total);
	return info;
}

void Kill_Quest::save(std::ofstream &file) {
	file << FILE_QUEST_ID << " " << _id << " "
		<< FILE_QUEST_ACTIVE << " " << _active << " "
		<< FILE_QUEST_COMPLETE << " " << _complete << " "
		<< FILE_QUEST_COUNT << " " << _count << " ";
}

void Kill_Quest::load(std::stringstream stream) {
	std::string key, data;
	while (stream >> key) {
		stream >> data;
		if (key == FILE_QUEST_ID) _id = std::stoi(data);
		else if (key == FILE_QUEST_ACTIVE) _active = (bool)std::stoi(data);
		else if (key == FILE_QUEST_COMPLETE) _complete = (bool)std::stoi(data);
		else if (key == FILE_QUEST_COUNT) _count = std::stoi(data);
	}
}