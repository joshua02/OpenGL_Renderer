#ifndef SPRITE_H
#define SPRITE_H


#include "gameObject.h"
#include "vec2.h"
#include "transform.h"
#include "textured_quad.h"

class Sprite : public GameObject {
public:
	Texture* texture{};

	Transform transform{};
	Vec2 pos{};
	Vec2 size{};
	int zIndex{};

	TexturedQuad rendererQuad;

	Sprite(Vec2 pos, Vec2 size, Texture* texture, int zIndex);
	~Sprite();
};

#endif