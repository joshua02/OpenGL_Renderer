#ifndef SPRITE_H
#define SPRITE_H


#include <vector>
#include <cmath>

#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <JAWEngine/vec2.h>

class Sprite {
public:
	Sprite(JAW::Vec2 pos, JAW::Vec2 size) : pos{ pos }, size{ size } {

		vertices.resize(24);
		vertices = {
			//positions						//colors			//texture coords
			-size.x/2, size.y/2, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			-size.x / 2, -size.y / 2, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
			size.x / 2, -size.y / 2, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
			size.x / 2, size.y / 2, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f
		};
		indices.resize(6);
		indices = {
			0, 1, 3,
			1, 2, 3
		};

		setupGeometry();
		vertices = {};
		indices = {};
	}

	void draw() const {
		shader->use();
		//shader->setUniform3f("ourColor", colR, colG, colB);
		shader->setUniform2f("tfPos", pos.x, pos.y);

		//glActiveTexture(GL_TEXTURE0);
		texture->use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
public:
	std::shared_ptr<Shader> shader{};
	std::shared_ptr<Texture> texture{};
	JAW::Vec2 pos{};
	JAW::Vec2 size{};

	float colR{};
	float colG{ 0.8f };
	float colB{ 0.8f };
	float colA{ 1.0f };
private:
	unsigned int VAO{};	//stores vertex attribute data
	unsigned int VBO{};	//stores vertex data
	unsigned int EBO{};	//stores index data
	std::vector<float> vertices{};
	std::vector<unsigned int> indices{};
	int indicesSize{6};

	void setupGeometry() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);

		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
};







#endif