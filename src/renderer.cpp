#include "renderer.h"

#include <chrono>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_menu.h"

#include <JAWEngine/vec2.h>

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
	testShader = std::make_shared<Shader>(RESOURCES_PATH "shaders/shader.vert", RESOURCES_PATH "shaders/shader.frag");
	textureShader = std::make_shared<Shader>(RESOURCES_PATH "shaders/textureShader.vert", RESOURCES_PATH "shaders/textureShader.frag");
	testTexture = std::make_shared<Texture>(RESOURCES_PATH "images/dog6.jpg");
}

void Renderer::setupGeometry() {
	Sprite& spr = sprites.emplace_back(JAW::Vec2{ 400.0f, 400.0f }, JAW::Vec2{ 200.0f, 200.0f });
	spr.setupGeometry();
	spr.texture = testTexture;
	spr.shader = textureShader;

	Sprite& spr2 = sprites.emplace_back(JAW::Vec2{ 0.0f, 0.0f }, JAW::Vec2{ 200.0f, 200.0f });
	spr2.setupGeometry();
	spr2.shader = textureShader;
	spr2.texture = testTexture;
	spr2.zIndex = 1;
}

void Renderer::drawFrame() {
	glClearColor(imguiMenu.clear_color.x, imguiMenu.clear_color.y, imguiMenu.clear_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO: change sprites to vector of "drawable" inheriting classes with draw method
	//TODO: batch drawable objects into a single VBO object and only store offsets
	for (const Sprite& spr : sprites) {
		spr.draw(projMatrix, viewMatrix);
	}
}