#ifndef GAME_H
#define GAME_H

#include "renderer.h"

class Game {
public:
	void init();
	void run();
	void gameLoop(float dt);
private:
	Renderer renderer{};

	bool running{ true };
};

#endif