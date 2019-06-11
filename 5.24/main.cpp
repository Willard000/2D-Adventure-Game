#include <iostream>
#include <windows.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Engine.h"
#include "Player.h"

int main(int args, char *argc[]) {

	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "SDL_LOADED" << std::endl;
	}
	if (!TTF_Init()) {
		std::cout << "SDL_TTF_LOADED" << std::endl;
	}

	lua_State *L = lua_open();
	luaL_openlibs(L);
	lua_openPlayer(L);

	Player player(3);

	luaW_push<Player>(L, &player);
	lua_setglobal(L, "ThePlayer");

	luaL_dofile(L, "Data/Lua/test.lua");

	lua_close(L);

	Engine engine;
	engine.run();

	TTF_Quit();
	SDL_Quit();
	return 0;
}