#ifndef TEXTURED_QUAD_H
#define TEXTURED_QUAD_H


#include <glm/glm.hpp>
#include <glad/glad.h>
#include <JAWEngine/vec2.h>

#include "scene.h"
#include "transform.h"

class TexturedQuad : public Drawable {
public:
	TexturedQuad(Transform* tf, Texture* texture, int& zIndex) : transform{ tf }, texture{ texture }, Drawable{ zIndex, Drawable::TRANSPARENT } {}

	Transform* transform;
	Texture* texture;

	void draw(glm::mat4 proj, glm::mat4 view) const override {
		shader->use();
		//shader->setUniform3f("ourColor", colR, colG, colB);

		//TODO: wrap glm::mat4 as a transform and updates to pos, scale, etc. modify directly

		shader->setUniformMatrix4fv("transform", transform->mat);
		shader->setUniformMatrix4fv("proj", proj);
		shader->setUniformMatrix4fv("view", view);
		shader->setUniform4f("tintColor", 1.0f, 1.0f, 1.0f, 1.0f);
		texture->use();
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	void setupGeometry() override {
		std::array<float, 32> vertices{
			//positions						//colors			//texture coords
			-0.5f, 0.5f, 0,		0.0f, 0.0f,
			-0.5f, -0.5f, 0,	0.0f, 1.0f,
			0.5f, -0.5f, 0,		1.0f, 1.0f,
			0.5f, 0.5f, 0,		1.0f, 0.0f
		};
		std::array<unsigned int, 6> indices{
			0, 1, 3,
			1, 2, 3
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
};


#endif