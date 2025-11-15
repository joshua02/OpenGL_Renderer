#ifndef CUBE_H
#define CUBE_H

#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>


class Cube {
public:
	Cube() : transform(1.0f) {
		vertices.resize(8 * 6);
		vertices = {
			//positions				//colors			//texture coords
			-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
			0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
			0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f,

			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
			0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f
		};
		indices.resize(36);
		indices = {
			1, 0, 3,
			1, 2, 3,
			2, 3, 7,
			2, 6, 7,
			6, 7, 4,
			6, 5, 4,
			5, 4, 0,
			5, 1, 0,
			3, 0, 4,
			3, 7, 4,
			2, 1, 5,
			2, 6, 5
		};

		setupGeometry();
		vertices = {};
		indices = {};
	}

	void draw() const {
		shader->use();
		//shader->setUniform3f("ourColor", colR, colG, colB);
		shader->setUniformMatrix4fv("model", transform);

		//glActiveTexture(GL_TEXTURE0);
		texture->use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void updateProj(const glm::mat4& proj) {
		shader->use();
		shader->setUniformMatrix4fv("projection", proj);
	}
	void updateView(const glm::mat4& view) {
		shader->use();
		shader->setUniformMatrix4fv("view", view);
	}

public:
	std::shared_ptr<Shader> shader{};
	std::shared_ptr<Texture> texture{};

	glm::mat4 transform;
	glm::mat4 proj;
	glm::mat4 view;

private:
	unsigned int VAO{};	//stores vertex attribute data
	unsigned int VBO{};	//stores vertex data
	unsigned int EBO{};	//stores index data
	std::vector<float> vertices{};
	std::vector<unsigned int> indices{};

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
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}



};


#endif