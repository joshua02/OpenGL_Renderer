#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

struct Transform {
	union {
		glm::mat4 mat{ 1.0f };
		struct {
			glm::vec4 col1;
			glm::vec4 col2;
			glm::vec4 col3;
			glm::vec3 position;
			float w;
		};
	};
};




#endif