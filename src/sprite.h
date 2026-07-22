#ifndef SPRITE_H
#define SPRITE_H


#include <array>
#include <cmath>

#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <JAWEngine/vec2.h>

#include <glm/glm.hpp>
#include "transform.h"

class Sprite {
public:
	std::shared_ptr<Shader> shader{};
	std::shared_ptr<Texture> texture{};

	Transform transform{};

	glm::vec3& position{ transform.position };

	JAW::Vec2 pos{};
	JAW::Vec2 size{};

	int zIndex{};

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
	//TODO: add and inherit from "drawable" object
	

};

#endif