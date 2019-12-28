#include <fstream>
#include <string>
#include <sstream>

#ifndef QUEST_H
#define QUEST_H

#define QUEST_TYPE_KILL 0

class Quest {
public:
	Quest(int id) :
		_id				( id ),
		_active		    ( 0 ),
		_complete		( 0 )
	{}
	virtual int get_type() = 0;
	virtual void save(std::ofstream &file) = 0;
	virtual void load(std::stringstream stream) = 0;
protected:
	int _id;
	bool _active;
	bool _complete;
};

#endif