#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "polygon.h"
#include "sprite.h"
#include <JAWEngine/vec2.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"

using namespace JAW;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 800;

void throwError(std::string_view error) {
	throw std::runtime_error(error.data());
}

class Renderer {
public:
	void run() {
		initWindow();
		
		otherInit();

		loadShaders();
		setupGeometry();
		

		initImGui();

		using ms = std::chrono::duration<float, std::milli>;

		auto lastFrameTime{ std::chrono::steady_clock::now() };
		
		while (running) {
			ms dt{std::chrono::steady_clock::now() - lastFrameTime};
			lastFrameTime = std::chrono::steady_clock::now();
			float dtSeconds{ dt.count() / 1000.0f };

			mainLoop(dtSeconds);
			gameLoop(dtSeconds);
		}
		
		cleanup();
	}

private:
	//SDL stuff
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

	void initWindow() {
		if (SDL_Init(SDL_INIT_VIDEO) != 1) {
			throwError(std::string("SDL_Init Error: ") + SDL_GetError());
		}

		//4x MSAA
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		window = SDL_CreateWindow(
			"OpenGL Window",
			WIDTH,
			HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		if (!window) {
			throwError(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		

		context = SDL_GL_CreateContext(window);
		SDL_GL_SetSwapInterval(1); //disable vsync with 0, enable with 1

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			throwError("Failed to initialize GLAD");
		}

		glViewport(0, 0, WIDTH, HEIGHT);


		glEnable(GL_MULTISAMPLE);
	}

	void initImGui() {
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

	void imguiFrame() {
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

	void otherInit() {
		glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		vec = trans * vec;
		std::cout << vec.x << vec.y << vec.z << std::endl;

		

	}

	void mainLoop(float dt) {
		//Poll events
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			switch (event.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					if (SDL_GetWindowID(window) == event.window.windowID) {
						glViewport(0, 0, event.window.data1, event.window.data2);
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

	void gameLoop(float dt) {
		const bool* snapshot = SDL_GetKeyboardState(nullptr);

		const float speed{ 1.0f };

		Vec2 vel{};

		if (snapshot[79]) {
			vel.x += 1;
		}
		if (snapshot[80]) {
			vel.x -= 1;
		}
		if (snapshot[81]) {
			vel.y -= 1;
		}
		if (snapshot[82]) {
			vel.y += 1;
		}
		sprite->pos += vel.normalize() * speed * dt;

		static float accTime{};
		accTime += dt;

		glm::mat4 trans(1.0f);
		
		trans = glm::translate(trans, glm::vec3(sprite->pos.x, sprite->pos.y, 0.0f));
		trans = glm::rotate(trans, glm::radians(accTime * 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 1.0f));

		sprite->transform = trans;
	}

	void cleanup() {
		SDL_GL_DestroyContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void loadShaders() {
		testShader = std::make_shared<Shader>(RESOURCES_PATH "shaders/shader.vert", RESOURCES_PATH "shaders/shader.frag");
		textureShader = std::make_shared<Shader>(RESOURCES_PATH "shaders/textureShader.vert", RESOURCES_PATH "shaders/textureShader.frag");
		testTexture = std::make_shared<Texture>(RESOURCES_PATH "images/dog6.jpg");
	}

	

	void setupGeometry() {
		square = std::make_unique<Polygon>(4, Vec2{0.5f, 0.0f}, 0.2f);
		square->shader = testShader;

		pentagon = std::make_unique<Polygon>(5, Vec2{-0.5f, 0.0f}, 0.6f);
		pentagon->shader = testShader;
		pentagon->colB = 0.2f;




		sprite = std::make_unique<Sprite>(Vec2{ 0.0f, 0.0f }, Vec2{0.5f, 0.5f});
		sprite->shader = textureShader;
		sprite->texture = testTexture;
	}

	void drawFrame() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		square->draw();
		pentagon->draw();
		sprite->draw();
	}
};




int main() {
	Renderer renderer;

	try {
		renderer.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
}

