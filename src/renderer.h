#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <SDL3/SDL.h>

#include "shader.h"
#include "polygon.h"
#include "sprite.h"
#include "imgui_menu.h"
#include "scene.h"

#include <JAWEngine/vec2.h>

class Renderer {
public:
	Renderer();
	void init();
	void run();
	void renderLoop(float dt);
	void cleanup();

	std::vector<Sprite> sprites{};
	std::vector<Line> lines{};

	bool running{ true };
	ImGuiMenu imguiMenu{};

	void addDrawable(std::unique_ptr<Drawable> drawable) {
		drawables.push_back(std::move(drawable));
	}

private:
	//SDL
	SDL_Window* window{ nullptr };
	SDL_Event event{};
	SDL_GLContext context{};
	
	//Renderer
	
	glm::mat4 projMatrix{};
	glm::mat4 viewMatrix{};

	std::vector<std::unique_ptr<Drawable>> drawables;

	void initWindow(std::uint32_t width = 800, std::uint32_t height = 600);

	void setupGeometry();
	void drawFrame();


};


#endif