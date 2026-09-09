#include "game.h"
#include "renderer.h"

#include "sprite.h"
#include "scene.h"
#include "player_character.h"

#include <chrono>
#include <thread>

void Game::init() {
	renderer.init();

	// Register actions

	inputManager.setActionKeys(Action::MoveLeft, {SDL_SCANCODE_A, SDL_SCANCODE_LEFT});
	inputManager.setActionKeys(Action::MoveRight, { SDL_SCANCODE_D, SDL_SCANCODE_RIGHT });
	inputManager.setActionKeys(Action::MoveUp, { SDL_SCANCODE_W, SDL_SCANCODE_UP });
	inputManager.setActionKeys(Action::MoveDown, { SDL_SCANCODE_S, SDL_SCANCODE_DOWN });

	inputManager.printActions();

	// Add sprites to the scene

	AssetLoader& assets{ AssetLoader::getInstance() };

	std::unique_ptr<Sprite> cat = std::make_unique<Sprite>(
		JAW::Vec2{ 100.0f, 100.0f },
		JAW::Vec2{ 200.0f, 200.0f },
		assets.getTexture("images/dog6.jpg"),
		0
	);
	renderer.addDrawable(cat.get());
	scene.getRoot()->addChild(std::move(cat));
	

	std::unique_ptr<Sprite> pixelArt = std::make_unique<Sprite>(
		JAW::Vec2{ 300.0f, 300.0f },
		JAW::Vec2{ 200.0f, 200.0f },
		assets.getTexture("images/pixel_test.png"),	//TODO: needs nearest neighbor filter
		10
	);
	renderer.addDrawable(pixelArt.get());
	scene.getRoot()->addChild(std::move(pixelArt));

	std::unique_ptr<PlayerCharacter> pika = std::make_unique<PlayerCharacter>(
		JAW::Vec2{ 300.0f, 300.0f },
		JAW::Vec2{ 100.0f, 100.0f },
		assets.getTexture("images/Pikachu.png"),
		20);
	renderer.addDrawable(pika.get());
	scene.getRoot()->addChild(std::move(pika));
	
	//TODO: create method in scene to add gameobjects and handle adding drawables to renderer
	//TODO: scene swapping
	//TODO: add dummy game object to hold lines, renderer holds list of pointers, so object needs to persist
	//TODO: handle deleting objects, need to make sure renderer doesn't try to dereference pointer after deletion

	// test draw lines

	//for (int i = 0; i < 20; i++) {
	//	Line line{ JAW::Vec2{ i * 20.0f, 20.0f }, JAW::Vec2{ i * 40.0f, 200.0f }, 0 };
	//	line.width = 3.0f;
	//	line.colB = i * 0.02f;
	//	renderer.addDrawable(&line);
	//}

}

void Game::run() {
	//TODO: run render loop and game loop in seperate threads
	using ms = std::chrono::duration<float, std::milli>;
	using namespace std::chrono_literals;

	auto lastFrameTime{ std::chrono::steady_clock::now() };

	while (running) {
		ms dt{ std::chrono::steady_clock::now() - lastFrameTime };
		lastFrameTime = std::chrono::steady_clock::now();
		float dtSeconds{ dt.count() / 1000.0f };

		inputManager.process();
		renderer.renderLoop(dtSeconds);
		gameLoop(dtSeconds);
		process(dtSeconds);
		running = renderer.running;
	}

	renderer.cleanup();
}

void Game::gameLoop(float dt) {

}

// Game objects are processed starting at scene root node and recursively called down to each child

void Game::process(float dt) {
	for (std::unique_ptr<GameObject>& go : scene.getRoot()->getChildren()) {
		go->process(dt);
		go->processChildren(dt);
	}
}