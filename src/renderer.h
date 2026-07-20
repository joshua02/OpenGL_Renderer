#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <SDL3/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include "shader.h"
#include "polygon.h"
#include "sprite.h"

class Renderer {
public:
	void init();
	void run();
private:
	//SDL
	SDL_Window* window{ nullptr };
	SDL_Event event{};
	SDL_GLContext context{};

	//geometry
	std::unique_ptr<Polygon> square{ nullptr };
	std::unique_ptr<Polygon> pentagon{ nullptr };
	std::unique_ptr<Sprite> sprite{ nullptr };

	//shaders
	std::shared_ptr<Shader> testShader{ nullptr };
	std::shared_ptr<Shader> textureShader{ nullptr };
	std::shared_ptr<Texture> testTexture{ nullptr };

	//ImGui stuff
	ImGuiIO* io{ nullptr };
	bool show_demo_window{ true };
	bool show_another_window{ false };
	ImVec4 clear_color{ ImVec4(0.45f, 0.55f, 0.60f, 1.00f) };

	//Renderer
	bool running{ true };
	glm::mat4 projMatrix{};
	glm::mat4 viewMatrix{ glm::mat4(1.0f) };

	void initWindow(std::uint32_t width = 800, std::uint32_t height = 600);
	void initImGui();
	void imguiFrame();
	void otherInit();
	void mainLoop(float dt);
	void gameLoop(float dt);
	void cleanup();
	void loadShaders();
	void setupGeometry();
	void drawFrame();


};


#endif