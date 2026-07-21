#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include <SDL3/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include "shader.h"
#include "polygon.h"
#include "sprite.h"

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
private:
	//SDL
	SDL_Window* window{ nullptr };
	SDL_Event event{};
	SDL_GLContext context{};

	//geometry
	
	Sprite sprite{ {0.0f, 0.0f}, {200.0f, 200.0f} };

	//shaders
	std::shared_ptr<Shader> testShader{ nullptr };
	std::shared_ptr<Shader> textureShader{ nullptr };
	std::shared_ptr<Texture> testTexture{ nullptr };

	//ImGui stuff
	ImGuiIO* io{ nullptr };
	bool show_demo_window{ true };
	bool show_another_window{ false };
	ImVec4 clear_color{ ImVec4(0.2f, 0.3f, 0.3f, 1.0f) };

	//Renderer
	
	glm::mat4 projMatrix{};
	glm::mat4 viewMatrix{};

	void initWindow(std::uint32_t width = 800, std::uint32_t height = 600);
	void initImGui();
	void imguiFrame();
	
	void loadShaders();
	void setupGeometry();
	void drawFrame();


};


#endif