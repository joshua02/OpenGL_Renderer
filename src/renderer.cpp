#include "renderer.h"

#include <chrono>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_menu.h"

#include <JAWEngine/vec2.h>

#include "asset_loader.h"

Renderer::Renderer() {

}

void Renderer::init() {
	initWindow();
	loadShaders();
	setupGeometry();
	imguiMenu.imguiInit(window, &context);
}

void Renderer::run() {
	
}

void Renderer::initWindow(std::uint32_t width, std::uint32_t height) {
	if (SDL_Init(SDL_INIT_VIDEO) != 1) {
		throw std::runtime_error(SDL_GetError());
	}

	//4x MSAA
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow(
		"OpenGL Window",
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	if (!window) {
		throw std::runtime_error(SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); //disable vsync with 0, enable with 1

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glViewport(0, 0, width, height);
	projMatrix = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -100.0f, 100.0f);
	//projMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	viewMatrix = glm::translate(glm::mat4{1.0f}, glm::vec3(0.0f, 0.0f, -3.0f));
	//projMatrix = glm::mat4(1.0f);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderLoop(float dt) {
	//Poll events
	//TODO: does it make sense to move this to game class?
	while (SDL_PollEvent(&event)) {
		imguiMenu.processEvent(&event);
		
		switch (event.type) {
		case SDL_EVENT_QUIT:
			running = false;
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			if (SDL_GetWindowID(window) == event.window.windowID) {
				glViewport(0, 0, event.window.data1, event.window.data2);
				projMatrix = glm::ortho(0.0f, static_cast<float>(event.window.data1), 0.0f, static_cast<float>(event.window.data2), -100.0f, 100.0f);
			}
			break;
		}
	}
	//rendering
	drawFrame();
	imguiMenu.imguiFrame();

	//swap buffers
	SDL_GL_SwapWindow(window);

}

void Renderer::cleanup() {
	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
//TODO: move shader loading to seperate resource loader
void Renderer::loadShaders() {

	AssetLoader& assets{ AssetLoader::getInstance() };

	testShader = assets.getShader("shaders/shader.vert", "shaders/shader.frag");
	textureShader = assets.getShader("shaders/textureShader.vert", "shaders/textureShader.frag");
	lineShader = assets.getShader("shaders/lineShader.vert", "shaders/lineShader.frag");

	testTexture = assets.getTexture("images/dog6.jpg");
	transparentTexture = assets.getTexture("images/Pikachu.png");
	pixelTexture = assets.getTexture("images/pixel_test.png"); //TODO: needs nearest filter
}

void Renderer::setupGeometry() {
	
	Sprite& cat = sprites.emplace_back(JAW::Vec2{ 0.0f, 0.0f }, JAW::Vec2{ 200.0f, 200.0f }, 0);
	cat.setupGeometry();
	cat.shader = textureShader;
	cat.texture = testTexture;

	Sprite& pixelArt = sprites.emplace_back(JAW::Vec2{ 300.0f, 300.0f }, JAW::Vec2{ 200.0f, 200.0f }, 10);
	pixelArt.setupGeometry();
	pixelArt.texture = pixelTexture;
	pixelArt.shader = textureShader;

	Sprite& pika = sprites.emplace_back(JAW::Vec2{ 300.0f, 300.0f }, JAW::Vec2{ 100.0f, 100.0f }, 20);
	pika.setupGeometry();
	pika.texture = transparentTexture;
	pika.shader = textureShader;

	for (int i = 0; i < 20; i++) {
		Line& line = lines.emplace_back();
		line.width = 3.0f;
		line.x1 = i*20.0f;
		line.y1 = 20.0f;
		line.x2 = i*40.0f;
		line.y2 = 200.0f;
		line.colB = i * 0.02f;
		line.setupGeometry();
		line.shader = lineShader;
	}
	
	
}

void Renderer::drawFrame() {
	glClearColor(imguiMenu.clear_color.x, imguiMenu.clear_color.y, imguiMenu.clear_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO: change sprites to vector of "drawable" inheriting classes with draw method
	//TODO: batch drawable objects into a single VBO object and only store offsets

	std::sort(sprites.begin(), sprites.end(), [](const Sprite& a, const Sprite& b) {
		return a.zIndex < b.zIndex;
	});
	for (const Line& line : lines) {
		line.draw(projMatrix, viewMatrix);
	}
	for (const Sprite& spr : sprites) {
		spr.draw(projMatrix, viewMatrix);
	}
	
}