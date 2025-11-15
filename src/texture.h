#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

class Texture {
public:
	Texture(std::string_view texturePath) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		//TODO: filtering options...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		int width{};
		int height{};
		int nrChannels{};
		unsigned char* data = stbi_load(texturePath.data(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			throw std::runtime_error("texture file failed to load");
		}
		stbi_image_free(data);
	}

	void use() const {
		glBindTexture(GL_TEXTURE_2D, id);
	}

public:
	unsigned int id{};

};







#endif