#include "sprite.h"
#include "asset_loader.h"
#include "renderer.h"

Sprite::Sprite(JAW::Vec2 pos, JAW::Vec2 size, Texture* texture, int zIndex) : pos{ pos }, size{ size }, texture{ texture }, rendererQuad{ transform, this->texture, size, this->zIndex } {

	rendererQuad.shader = AssetLoader::getInstance().getShader("shaders/textureShader.vert", "shaders/textureShader.frag");

	transform.mat = glm::translate(transform.mat, glm::vec3{ pos.x, pos.y, zIndex });

	Renderer::getInstance().addDrawable(&rendererQuad);

}

Sprite::~Sprite() {

}

