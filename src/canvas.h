#ifndef CANVAS_H
#define CANVAS_H

#include "scene.h"
#include <vector>
#include <memory>

#include "renderer.h"

#include <JAWEngine/vec2.h>

class Canvas : public GameObject{
public:
	void addDrawable(std::unique_ptr<Drawable> drawable) {
		Renderer::getInstance().addDrawable(drawable.get());
		drawables.push_back(std::move(drawable));
	}

private:
	std::vector<std::unique_ptr<Drawable>> drawables;


};




#endif