#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "gameObject.h"

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