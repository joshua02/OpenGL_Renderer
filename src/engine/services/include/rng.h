#ifndef RNG_H
#define RNG_H

#include <random>

namespace RNG {

	std::mt19937 generator;

	void setSeed(unsigned int seed) {
		generator = std::mt19937{ seed };
	}

	void seedEntropy() {
		std::random_device rd;
		generator = std::mt19937{ rd() };
	}

	int intRange(int low, int high) {
		std::uniform_int_distribution<int> distribution{ low, high };
		return distribution(generator);
	}

	float floatRange(float low, float high) {
		std::uniform_real_distribution<float> distribution{ low, high };
		return distribution(generator);
	}
}

#endif