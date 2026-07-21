#include "game.h"
#include "renderer.h"

#include <chrono>

void Game::init() {
	renderer.init();
}

void Game::run() {
	//TODO: run render loop and game loop in seperate threads
	using ms = std::chrono::duration<float, std::milli>;

	auto lastFrameTime{ std::chrono::steady_clock::now() };

	while (running) {
		ms dt{ std::chrono::steady_clock::now() - lastFrameTime };
		lastFrameTime = std::chrono::steady_clock::now();
		float dtSeconds{ dt.count() / 1000.0f };

		renderer.renderLoop(dtSeconds);
		gameLoop(dtSeconds);
		running = renderer.running;
	}

	renderer.cleanup();
}

void Game::gameLoop(float dt) {
	const bool* snapshot = SDL_GetKeyboardState(nullptr);

	const float speed{ 500.0f };

	JAW::Vec2 vel{};

	if (snapshot[SDL_SCANCODE_RIGHT]) {
		vel.x += 1;
	}
	if (snapshot[SDL_SCANCODE_LEFT]) {
		vel.x -= 1;
	}
	if (snapshot[SDL_SCANCODE_DOWN]) {
		vel.y -= 1;
	}
	if (snapshot[SDL_SCANCODE_UP]) {
		vel.y += 1;
	}
	renderer.sprites[1].pos += vel.normalize() * speed * dt;

	static float accTime{};
	accTime += dt;

	//trans = glm::translate(trans, glm::vec3(sprite.pos.x, sprite.pos.y, 0.0f));
	//trans = glm::rotate(trans, glm::radians(accTime * 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}