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

	glm::vec3 vel{};

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
	renderer.sprites[1].transform.position += vel * speed * dt;

	static float accTime{};
	accTime += dt;

	renderer.sprites[0].transform.position = glm::vec3{ renderer.imguiMenu.pos[0], renderer.imguiMenu.pos[1], renderer.imguiMenu.pos[2] };
}