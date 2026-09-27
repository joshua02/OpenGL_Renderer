#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "sprite.h"
#include "input_manager.h"

#include <JAWEngine/vec2.h>

#include <numbers>

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

		if (input.actionJustStarted(Action::MoveUp)) {
			transform.setScale(1.5, 1.5);
		}
		if (input.actionJustEnded(Action::MoveUp)) {
			transform.setScale(1.0, 1.0);
		}

		if (input.actionJustStarted(Action::MoveDown)) {
			transform.setScale(0.5, 0.5);
		}
		if (input.actionJustEnded(Action::MoveDown)) {
			transform.setScale(1.0, 1.0);
		}

		if (input.actionIsPressed(Action::MoveLeft)) {
			transform.setRotation(transform.rotation + std::numbers::pi / 180 * -1 * speed * dt);
		}
		if (input.actionIsPressed(Action::MoveRight)) {
			transform.setRotation(transform.rotation + std::numbers::pi / 180 * 1 * speed * dt);
		}

		transform.position.x += vel.x;
		transform.position.y += vel.y;
	}
};


#endif