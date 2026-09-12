#ifndef SPRITE_H
#define SPRITE_H


#include <array>
#include <cmath>

#include "shader.h"
#include "texture.h"
#include "scene.h"

#include <glad/glad.h>
#include <JAWEngine/vec2.h>

#include <glm/glm.hpp>
#include "transform.h"
#include "textured_quad.h"

class Sprite : public GameObject {
public:
	Texture* texture{};

	Transform transform{};
	JAW::Vec2 pos{};
	JAW::Vec2 size{};
	int zIndex{};

	TexturedQuad rendererQuad;

	Sprite(JAW::Vec2 pos, JAW::Vec2 size, Texture* texture, int zIndex);
	~Sprite();
};

#endif