#include "sprite.h"
#include "asset_loader.h"
#include "renderer.h"

Sprite::Sprite(Vec2 pos, Vec2 size, Texture* texture, int zIndex) : pos{ pos }, size{ size }, texture{ texture }, rendererQuad{ &transform, this->texture, this->zIndex } {

	rendererQuad.shader = AssetLoader::getInstance().getShader("shaders/textureShader.vert", "shaders/textureShader.frag");

	transform.mat = glm::translate(transform.mat, glm::vec3{ pos.x, pos.y, zIndex });
	transform.mat = glm::scale(transform.mat, glm::vec3{size.x, size.y, 0.0f});

	Renderer::getInstance().addDrawable(&rendererQuad);

}

Sprite::~Sprite() {

}

