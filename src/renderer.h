#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <SDL3/SDL.h>

#include "shader.h"
#include "polygon.h"
#include "sprite.h"
#include "imgui_menu.h"

#include <JAWEngine/vec2.h>

class Renderer {
public:
	Renderer();
	void init();
	void run();
	void renderLoop(float dt);
	void cleanup();

	std::vector<Sprite> sprites{};
	bool running{ true };
	ImGuiMenu imguiMenu{};
private:
	//SDL
	SDL_Window* window{ nullptr };
	SDL_Event event{};
	SDL_GLContext context{};

	//shaders
	std::shared_ptr<Shader> testShader{ nullptr };
	std::shared_ptr<Shader> textureShader{ nullptr };
	std::shared_ptr<Texture> testTexture{ nullptr };



	//Renderer
	
	glm::mat4 projMatrix{};
	glm::mat4 viewMatrix{};

	void initWindow(std::uint32_t width = 800, std::uint32_t height = 600);

	void loadShaders();
	void setupGeometry();
	void drawFrame();


};


#endif