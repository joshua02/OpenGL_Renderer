#ifndef SPRITE_H
#define SPRITE_H


#include <array>
#include <cmath>

#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <JAWEngine/vec2.h>

#include <glm/glm.hpp>

class Sprite {
public:
	std::shared_ptr<Shader> shader{};
	std::shared_ptr<Texture> texture{};

	JAW::Vec2 pos{};
	JAW::Vec2 size{};

	glm::mat4 transform{};
	float colR{};
	float colG{ 0.8f };
	float colB{ 0.8f };
	float colA{ 1.0f };

	Sprite(JAW::Vec2 pos, JAW::Vec2 size);

	void draw(glm::mat4 proj, glm::mat4 view) const;

	void printTransform() const;

	void setupGeometry();
private:
	unsigned int VAO{};	//stores vertex attribute data
	unsigned int VBO{};	//stores vertex data
	unsigned int EBO{};	//stores index data

	

};

#endif