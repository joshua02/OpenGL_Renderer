#ifndef POLYGON_H
#define POLYGON_H


#include <vector>
#include <cmath>

#include "shader.h"

#include <glad/glad.h>
#include <JAWEngine/vec2.h>

class Polygon {
public:
	Polygon(int sides, JAW::Vec2 center, float rad) : pos{ center }, vertices(3 * (sides + 1)), indices(3 * sides), radius{ rad } {
		if (sides == 3) {
			vertices.resize(9);
			vertices = {
				rad * std::cosf(1.57f + 0.0f / sides * 6.28f), rad * std::sinf(1.57f + 0.0f / sides * 6.28f), 0.0f,
				rad * std::cosf(1.57f + 1.0f / sides * 6.28f), rad * std::sinf(1.57f + 1.0f / sides * 6.28f), 0.0f,
				rad * std::cosf(1.57f + 2.0f / sides * 6.28f), rad * std::sinf(1.57f + 2.0f / sides * 6.28f), 0.0f
			};
			indices.resize(3);
			indices = { 0, 1, 2 };
		} else if (sides == 4) {
			vertices.resize(12);
			vertices = {
				rad / std::sqrtf(2), rad / std::sqrtf(2), 0.0f,
				rad / std::sqrtf(2), -rad / std::sqrtf(2), 0.0f,
				-rad / std::sqrtf(2), -rad / std::sqrtf(2), 0.0f,
				-rad / std::sqrtf(2), rad / std::sqrtf(2), 0.0f,
			};
			indices.resize(6);
			indices = {
				0, 1, 3,
				1, 2, 3
			};
		} else if (sides > 4) {
			vertices[0] = 0.0f;
			vertices[1] = 0.0f;
			vertices[2] = 0.0f;
			for (int i = 0; i < sides; i++) {
				vertices[3+i*3] = (rad * std::cosf(1.57f + static_cast<float>(i) / sides * 6.28f));
				vertices[4+i*3] = (rad * std::sinf(1.57f + static_cast<float>(i) / sides * 6.28f));
				vertices[5+i*3] = 0.0f;
			}
			for (int i = 0; i < sides-1; i++) {
				indices[i*3] = 0;
				indices[1 + i * 3] = i + 1;
				indices[2 + i*3] = i + 2;
			}
			indices[(sides-1) * 3] = 0;
			indices[1 + (sides-1) * 3] = sides;
			indices[2 + (sides-1) * 3] = 1;
		}
		indicesSize = indices.size();
		setupGeometry();
		vertices = {};
		indices = {};
	}

	void draw() const {
		shader->use();
		shader->setUniform4f("ourColor", colR, colG, colB, colA);
		shader->setUniform2f("tfPos", pos.x, pos.y);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
	}
public:
	std::shared_ptr<Shader> shader{};
	JAW::Vec2 pos{};
	float colR{};
	float colG{};
	float colB{ 0.8f };
	float colA{ 1.0f };
private:
	unsigned int VAO{};	//stores vertex attribute data
	unsigned int VBO{};	//stores vertex data
	unsigned int EBO{};	//stores index data
	std::vector<float> vertices{};
	std::vector<unsigned int> indices{};
	int indicesSize{};
	float radius{};

	void setupGeometry() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*indices.size(), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
};







#endif