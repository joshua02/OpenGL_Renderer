#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>

#include "shader.h"
#include "asset_loader.h"
#include <JAWEngine/vec2.h>

class GameObject {
public:
	GameObject* parent{ nullptr };
	
	virtual void process(float dt) {};

	void processChildren(float dt) {
		for (std::unique_ptr<GameObject>& go : getChildren()) {
			go->process(dt);
			go->processChildren(dt);
		}
	}

	void addChild(std::unique_ptr<GameObject> go) {
		children.push_back(std::move(go));
	}

	std::vector<std::unique_ptr<GameObject>>& getChildren() {
		return children;
	}

protected:
	std::vector<std::unique_ptr<GameObject>> children;
};

class Drawable {
public:

	Drawable(int zIndex) : zIndex { zIndex } {};

	Shader* shader{ nullptr };
	int zIndex{};

	virtual void draw(glm::mat4 proj, glm::mat4 view) const {
		throw std::runtime_error("Drawable derived class's draw method is not implemented");
	}
	virtual void setupGeometry() {
		throw std::runtime_error("Drawable derived class's draw method is not implemented");
	}
	
	virtual ~Drawable() = default;
protected:
	GLuint VAO{};	//stores vertex attribute data
	GLuint VBO{};	//stores vertex data
	GLuint EBO{};	//stores index data
};

class Line : public Drawable {
public:

	Line(JAW::Vec2 p1, JAW::Vec2 p2, int zIndex) : p1{ p1 }, p2{ p2 }, Drawable(zIndex) {

		shader = AssetLoader::getInstance().getShader("shaders/lineShader.vert", "shaders/lineShader.frag");
	};

	Shader* shader{ nullptr };

	float colR{ 0.5f };
	float colG{ 0.0f };
	float colB{ 0.0f };

	JAW::Vec2 p1;
	JAW::Vec2 p2;

	float width{ 5.0f };

	void draw(glm::mat4 proj, glm::mat4 view) const override {
		shader->use();
		shader->setUniform3f("ourColor", colR, colG, colB);

		shader->setUniformMatrix4fv("proj", proj);
		shader->setUniformMatrix4fv("view", view);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	void setupGeometry() override {

		//TODO: Use geometry shader to calculate vertices based on x1, y1, x2, y2, and width

		float mag{ std::powf(std::powf(p2.x - p1.x,2) + std::powf(p2.y - p1.y,2), 0.5f) };

		float dirX{ (p2.x - p1.x) / mag };
		float dirY{ (p2.y - p1.y) / mag };

		float normX{ -dirY };
		float normY{ dirX };

		std::array<float, 32> vertices{
			//positions
			p1.x + width * normX, p1.y + width * normY, zIndex,
			p2.x + width * normX, p2.y + width * normY, zIndex,
			p1.x - width * normX, p1.y - width * normY, zIndex,
			p2.x - width * normX, p2.y - width * normY, zIndex
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

	Scene() : root{std::make_unique<GameObject>()} {}

	GameObject* getRoot() {
		return root.get();
	}
private:
	std::unique_ptr<GameObject> root;
};


#endif