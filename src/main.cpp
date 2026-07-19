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
#include "renderer.h"

using namespace JAW;

int main() {
	Renderer renderer;

	try {
		renderer.init();
		renderer.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
}

