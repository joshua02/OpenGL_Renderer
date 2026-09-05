#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include "scene.h"

class Game {
public:
	void init();
	void run();
	void gameLoop(float dt);
private:
	Renderer renderer{};
	Scene scene{};

	bool running{ true };
};

#endif