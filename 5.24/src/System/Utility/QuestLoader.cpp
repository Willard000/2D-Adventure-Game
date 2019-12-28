#include "QuestLoader.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "PlayerComponent.h"
#include "Quest.h"
#include "Kill_Quest.h"
#include "FileReader.h"
#include "Log.h"
#include "Globals.h"

#define KILL_QUEST "Kill_Quest"

#define KILL_TARGET_ID "itarget_id"
#define KILL_TOTAL "itotal"

Kill_Quest *load_kill_quest(FileReader &file, int id) {
	int target_id = 0;
	int total = 0;

	if (file.exists(KILL_TARGET_ID)) target_id = file.get_int(KILL_TARGET_ID);
	if (file.exists(KILL_TOTAL)) total = file.get_int(KILL_TOTAL);

	return new Kill_Quest(id, target_id, total);
}

Quest *load_quest(int id) {
	std::string locate_file_path = QUEST_FOLDER + std::string("/") + "quests.txt";
	FileReader locate_file(locate_file_path.c_str());

	if (!locate_file.exists(std::to_string(id))) {
		Environment::get().get_log()->print(
			"No quest with id of: \""
			+ std::to_string(id)
			+ "\" exits."
		);
		return nullptr;
	}

	FileReader file(locate_file.get_string(std::to_string(id)).c_str());

	if (file.exists(KILL_QUEST))
		return load_kill_quest(file, id);

	return nullptr;
}

void load_quests() {
	PlayerComponent *player = GetPlayer(Environment::get().get_resource_manager()->get_player());
	
	int id = 0;
	for (auto &q : player->quest_log) {
		q = load_quest(id);
		++id;
	}
}