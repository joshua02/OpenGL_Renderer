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
	void renderLoop(float dt);
	void cleanup();

	bool running{ true };
	ImGuiMenu imguiMenu{};

	void addDrawable(Drawable* drawable);

private:
	//SDL
	SDL_Window* window{ nullptr };
	SDL_Event event{};
	SDL_GLContext context{};
	
	//Renderer
	
	glm::mat4 projMatrix{};
	glm::mat4 viewMatrix{};

	std::vector<Drawable*> drawables;

	void initWindow(std::uint32_t width = 800, std::uint32_t height = 600);
	void drawFrame();


};


#endif