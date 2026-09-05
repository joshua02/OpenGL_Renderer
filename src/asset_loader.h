#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <unordered_map>
#include <string>

#include "shader.h"
#include "texture.h"


/*

Asset loader exists to turn a file into a game asset including textures, shaders, 3d models, fonts

TODO:
all file paths are appended to the resources directory
subsequent calls to load functions return the already loaded asset

*/


class AssetLoader {
public:
	Shader* getShader(std::string_view vertPath, std::string_view fragPath) {
		if (auto it{ shaders.find(std::string{vertPath}) }; it != shaders.end()) {
			//shader already loaded
			std::cout << "shader already loaded\n";
			return &it->second;
		}
		else {
			return loadShader(vertPath, fragPath);
		}
	}
	Texture* getTexture(std::string_view texturePath) {
		if (auto it{ textures.find(std::string{texturePath}) }; it != textures.end()) {
			//texture already loaded
			std::cout << "texture already loaded\n";
			return &it->second;
		}
		else {
			return loadTexture(texturePath);
		}
	}
	void unloadTexture(const Texture& texture);
	void unloadShader(const Shader& shader);

	static AssetLoader& getInstance() {
		static AssetLoader instance{};

		return instance;
	}
private:
	std::unordered_map<std::string, Shader> shaders;
	std::unordered_map<std::string, Texture> textures;

	Shader* loadShader(std::string_view vertPath, std::string_view fragPath) {
		//auto [it, success] { shaders.emplace(std::string{ vertPath }, Shader{ RESOURCES_PATH + std::string{vertPath}, RESOURCES_PATH + std::string{fragPath} }) };
		auto [it, success] { shaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(std::string{ vertPath }),
			std::forward_as_tuple(RESOURCES_PATH + std::string{vertPath}, RESOURCES_PATH + std::string{fragPath})
		)};

		std::cout << it->first << " loaded\n";
		return &it->second;
	}
	Texture* loadTexture(std::string_view texturePath) {
		//auto [it, success] { textures.emplace(std::string{ texturePath }, Texture{ RESOURCES_PATH + std::string{texturePath} })};
		auto [it, success] { textures.emplace(
			std::piecewise_construct, 
			std::forward_as_tuple(std::string{ texturePath }), 
			std::forward_as_tuple(RESOURCES_PATH + std::string{texturePath})
		)};
		std::cout << it->first << " loaded\n";
		std::cout << "textures map has " << textures.size() << " elements\n";
		return &it->second;
	}
};



#endif