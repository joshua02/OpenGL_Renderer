#include "sprite.h"


void Sprite::draw(glm::mat4 proj, glm::mat4 view) const {
	shader->use();
	//shader->setUniform3f("ourColor", colR, colG, colB);
	shader->setUniformMatrix4fv("transform", transform);
	shader->setUniformMatrix4fv("proj", proj);
	shader->setUniformMatrix4fv("view", view);

	//glActiveTexture(GL_TEXTURE0);
	texture->use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sprite::printTransform() const {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			std::cout << transform[x][y] << ", ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Sprite::setupGeometry() {
	std::array<float, 32> vertices{
		//positions						//colors			//texture coords
		-size.x / 2, size.y / 2, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-size.x / 2, -size.y / 2, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		size.x / 2, -size.y / 2, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		size.x / 2, size.y / 2, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f
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


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}