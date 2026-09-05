#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cmath>

#include <glm/glm.hpp>

#include "shader.h"

class GameObject {
public:
	GameObject* parent{ nullptr };
	std::vector<GameObject*> children{};
};

class Drawable {
public:
	Shader* shader{ nullptr };
	virtual void draw(glm::mat4 proj, glm::mat4 view) const = 0;
	virtual void setupGeometry() = 0;
protected:
	GLuint VAO{};	//stores vertex attribute data
	GLuint VBO{};	//stores vertex data
	GLuint EBO{};	//stores index data
};

class Line : public Drawable {
public:
	Shader* shader{ nullptr };

	float colR{ 0.5f };
	float colG{ 0.0f };
	float colB{ 0.0f };

	float x1{};
	float y1{};
	float x2{};
	float y2{};
	float width{ 5.0f };

	void draw(glm::mat4 proj, glm::mat4 view) const {
		shader->use();
		shader->setUniform3f("ourColor", colR, colG, colB);

		shader->setUniformMatrix4fv("proj", proj);
		shader->setUniformMatrix4fv("view", view);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void setupGeometry() {

		//TODO: Use geometry shader to calculate vertices based on x1, y1, x2, y2, and width

		float mag{ std::powf(std::powf(x2 - x1,2) + std::powf(y2 - y1,2), 0.5f) };

		float dirX{ (x2 - x1) / mag };
		float dirY{ (y2 - y1) / mag };

		float normX{ -dirY };
		float normY{ dirX };

		std::array<float, 32> vertices{
			//positions
			x1 + width * normX, y1 + width * normY, 0,
			x2 + width * normX, y2 + width * normY, 0,
			x1 - width * normX, y1 - width * normY, 0,
			x2 - width * normX, y2 - width * normY, 0
		};
		std::array<unsigned int, 6> indices{
			0, 1, 3,
			0, 2, 3
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(0);
	}
};

//class Sprite : public GameObject, public Drawable {
//public:
//	 
//};

class Scene {
public:
	GameObject root;

};


#endif