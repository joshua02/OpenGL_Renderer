#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <vector>

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



#endif