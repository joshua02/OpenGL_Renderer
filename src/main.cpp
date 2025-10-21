#include <iostream>

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "shader.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void throwError(std::string error) {
	throw std::runtime_error(error);
}

float vertices[] = {
	-0.1f, 0.0f, 0.0f,
	-0.3f, 0.0f, 0.0f,
	-0.2f, 0.2f, 0.0f
};

float vertices2[] = {
	0.1f, 0.0f, 0.0f,
	0.3f, 0.0f, 0.0f,
	0.2f, 0.2f, 0.0f
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = ourColor;\n"
"}\0";

const char* fragmentShaderSourceYellow = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

class Renderer {
public:
	void run() {
		initWindow();

		setupGeometry();
		
		setupVertexShader();
		setupFragmentShader();
		createShaderProgram();
		
		loadShaders();

		initImGui();

		while (running) {
			mainLoop();
		}
		
		cleanup();
	}
private:
	SDL_Window* window;
	SDL_Event event;
	SDL_GLContext context;
	
	unsigned int VBO;
	unsigned int vertexShader;
	unsigned int fragmentShader, fragmentShaderYellow;
	unsigned int shaderProgram, shaderProgramYellow;
	unsigned int VAO;
	unsigned int EBO;

	Shader* testShader;

	ImGuiIO* io;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	unsigned int VBO2, VAO2;

	bool running = true;

	void initWindow() {
		if (SDL_Init(SDL_INIT_VIDEO) != 1) {
			throwError(std::string("SDL_Init Error: ") + SDL_GetError());
		}

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

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;
			static bool vsync = true;

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

	void mainLoop() {
		//Poll events
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			switch (event.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					glViewport(0, 0, event.window.data1, event.window.data2);
					break;
				}
			}
		
		//rendering
		drawFrame();
		imguiFrame();

		//swap buffers
		SDL_GL_SwapWindow(window);

	}

	void cleanup() {
		SDL_GL_DestroyContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void loadShaders() {
		testShader = new Shader(RESOURCES_PATH "shaders/shader.vert", RESOURCES_PATH "shaders/shader.frag");
	}

	void drawFrame() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		float timeValue = SDL_GetTicks()/1000.0f;
		float greenValue = sin(timeValue * 2.0f * 3.14f * 0.1f) / 4.0f + 0.5f;
		
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		//glUseProgram(shaderProgram);

		testShader->use();
		//testShader->setUniform4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		testShader->setUniform4f("ourColor", clear_color.x, clear_color.y, clear_color.z, clear_color.w);

		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);




		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	}

	void setupGeometry() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//second vbo
		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(VAO2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
	}

	void setupVertexShader() {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			throwError("Shader failed" + std::string(infoLog));
		}
	}
	void setupFragmentShader() {
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			throwError("Shader failed" + std::string(infoLog));
		}

		fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
		glCompileShader(fragmentShaderYellow);

		glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
			throwError("Shader failed" + std::string(infoLog));
		}
	}

	void createShaderProgram() {
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			throwError("Shader failed" + std::string(infoLog));
		}

		//yellow
		shaderProgramYellow = glCreateProgram();
		glAttachShader(shaderProgramYellow, vertexShader);
		glAttachShader(shaderProgramYellow, fragmentShaderYellow);
		glLinkProgram(shaderProgramYellow);

		glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
			throwError("Shader failed" + std::string(infoLog));
		}

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}


};




int main() {
	Renderer renderer;

	try {
		renderer.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}

