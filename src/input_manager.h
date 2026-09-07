#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <unordered_map>

#include <SDL3/SDL.h>


enum class Action {
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown
};

struct ActionKeys {
	std::vector<SDL_Scancode> keys{};
};

class InputManager {
public:

	std::unordered_map<Action, ActionKeys> actions;

	static InputManager& getInstance() {
		static InputManager instance{};

		return instance;
	}

	void process() {
		
	}

	//TODO: action start/end
	bool actionIsPressed(Action actionName) {
		auto it{ actions.find(actionName) };
		if (it == actions.end()) {
			throw std::runtime_error(static_cast<int>(actionName) + " missing entry in actions");
		}

		for (SDL_Scancode key : it->second.keys) {
			if (keyboardState[key] == true) {
				return true;
			}
		}
		return false;
	}

	void addAction(Action actionName, const std::vector<SDL_Scancode>& keys) {
		actions[actionName] = ActionKeys{keys};
	}
	
	void printActions() {
		std::cout << "Actions:\n";
		for (auto it = actions.begin(); it != actions.end();) {
			
			std::cout << "Action: " << static_cast<int>(it->first) << ", key codes = ";

			for (auto key : it->second.keys) {
				std::cout << key << " ";
			}
			std::cout << '\n';
			it++;
		}
	}


private:
	const bool* keyboardState{SDL_GetKeyboardState(nullptr)};
};



#endif