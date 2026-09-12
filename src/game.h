#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include "scene.h"
#include "input_manager.h"

class Game {
public:
	void init();
	void run();
	void process(float dt);

private:
	Renderer& renderer{Renderer::getInstance()};
	Scene scene{};
	InputManager& inputManager{InputManager::getInstance()};

	bool running{ true };
};

#endif