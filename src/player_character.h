#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "sprite.h"
#include "input_manager.h"

#include <JAWEngine/vec2.h>

class PlayerCharacter : public Sprite {
public:
	PlayerCharacter(JAW::Vec2 pos, JAW::Vec2 size, Texture* texture, int zIndex) : Sprite(pos, size, texture, zIndex) {}
	
	float speed{ 500.0f };

	void process(float dt) override {
		InputManager& input{ InputManager::getInstance() };

		JAW::Vec2 vel{};

		if (input.actionIsPressed(Action::MoveLeft)) {
			vel.x += -1;
		}
		if (input.actionIsPressed(Action::MoveRight)) {
			vel.x += 1;
		}
		if (input.actionIsPressed(Action::MoveUp)) {
			vel.y += 1;
		}
		if (input.actionIsPressed(Action::MoveDown)) {
			vel.y += -1;
		}
		vel = vel.normalize() * speed * dt;

		transform.position.x += vel.x;
		transform.position.y += vel.y;
	}
};


#endif