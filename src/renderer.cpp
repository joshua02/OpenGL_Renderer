#include "renderer.h"

#include <chrono>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <JAWEngine/vec2.h>

Renderer::Renderer() {

}

void Renderer::init() {
	initWindow();
	loadShaders();
	setupGeometry();
	initImGui();
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

void Renderer::initImGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(window, &context);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Renderer::imguiFrame() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Renderer Settings");
	static bool vsync = true;
	ImGui::Checkbox("VSync", &vsync);
	SDL_GL_SetSwapInterval(vsync);
	ImGui::End();

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);



		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);


		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}


void Renderer::renderLoop(float dt) {
	//Poll events
	//TODO: does it make sense to move this to game class?
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
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
	imguiFrame();

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
	glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO: change sprites to vector of "drawable" inheriting classes with draw method
	//TODO: batch drawable objects into a single VBO object and only store offsets
	for (const Sprite& spr : sprites) {
		spr.draw(projMatrix, viewMatrix);
	}
}