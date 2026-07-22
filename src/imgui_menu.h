#ifndef IMGUI_MENU_H
#define IMGUI_MENU_H

#include "imgui.h"
#include <SDL3/SDL.h>

class ImGuiMenu {
public:
	void imguiFrame();
	void imguiInit(SDL_Window* window, SDL_GLContext* context);
	void processEvent(SDL_Event*);
	ImVec4 clear_color{ ImVec4(0.2f, 0.3f, 0.3f, 1.0f) };


};


#endif