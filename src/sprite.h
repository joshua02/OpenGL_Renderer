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

class Sprite : public GameObject, public Drawable {
public:
	Texture* texture{};

	Transform transform{};
	JAW::Vec2 pos{};
	JAW::Vec2 size{};
	int zIndex{};

	Sprite(JAW::Vec2 pos, JAW::Vec2 size, int zIndex);

	void draw(glm::mat4 proj, glm::mat4 view) const override;

	void setupGeometry() override;

};

#endif