#include <iostream>

#include "game.h"

int main() {
	Game game{};

	try {
		game.init();
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "EXCEPTION THROWN: " << e.what() << std::endl;
		std::exit(1);
	}
}

