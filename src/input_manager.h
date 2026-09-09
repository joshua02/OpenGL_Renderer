#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <array>
#include <mutex>

#include <SDL3/SDL.h>

//TODO: add mouse events

enum class Action {
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,

	LAST_ACTION
};

struct ActionInfo {
	std::vector<SDL_Scancode> keys{};
	bool state{ false };
	bool lastState{ false };
};

class InputManager {
public:

	std::array<ActionInfo, static_cast<size_t>(Action::LAST_ACTION)> actions;

	static InputManager& getInstance() {
		static InputManager instance{};

		return instance;
	}

	void process() {

		std::lock_guard<std::mutex> lock{ mtx };

		for (size_t i = 0; i < static_cast<size_t>(Action::LAST_ACTION); i++) {
			ActionInfo& action = actions[i];

			action.lastState = action.state;
			action.state = false;
			for (SDL_Scancode key : action.keys) {
				if (keyboardState[key] == true) {
					action.state = true;
				}
			}
		}
	}

	bool actionIsPressed(Action actionName) {
		std::lock_guard<std::mutex> lock{ mtx };
		return actions[static_cast<size_t>(actionName)].state;
	}

	bool actionJustStarted(Action actionName) {
		std::lock_guard<std::mutex> lock{ mtx };
		ActionInfo& action = actions[static_cast<size_t>(actionName)];
		return (action.lastState == false && action.state == true);
	}

	bool actionJustEnded(Action actionName) {
		std::lock_guard<std::mutex> lock{ mtx };
		ActionInfo& action = actions[static_cast<size_t>(actionName)];
		return (action.lastState == true && action.state == false);
	}

	void setActionKeys(Action actionName, const std::vector<SDL_Scancode>& keys) {
		actions[static_cast<size_t>(actionName)] = ActionInfo{ keys };
	}

	void printActions() {
		std::cout << "Actions:\n";
		for (size_t i = 0; i < static_cast<size_t>(Action::LAST_ACTION) - 1; i++) {

			std::cout << "Action: " << i << ", key codes = ";

			for (auto key : actions[i].keys) {
				std::cout << key << " ";
			}
			std::cout << '\n';
		}
	}


private:
	const bool* keyboardState{ SDL_GetKeyboardState(nullptr) };

	/* 
		mutex so that when multithreading is added, calls to polling functions actionIsPressed, actionJustStarted, ...
		will return the correct values. If polling called in the middle of process, state and lastState may not be set yet
	*/
	std::mutex mtx;
};



#endif