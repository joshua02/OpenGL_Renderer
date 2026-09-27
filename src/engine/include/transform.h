#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
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

	glm::vec2 scale{1.0f, 1.0f};
	float rotation{0.0f};

	//TODO: does it make more sense to store an "unmodified" matrix and "transformed" matrix so 
	// transformed can be calculated from the scale/rotation without having to undo existing matrix?
	void setScale(float x, float y) {
		// Get normalized matrix by first scaling matrix down by current scale, then apply new scale to normalized matrix
		mat = glm::scale(glm::scale(mat, glm::vec3{1/scale.x, 1/scale.y, 1}), glm::vec3{ x, y, 0 });
		scale.x = x;
		scale.y = y;
	}

	void setRotation(float ang) {
		// Undo current rotation, then apply new rotation angle
		mat = glm::rotate(glm::rotate(mat, -rotation, glm::vec3{ 0, 0, -1 }), ang, glm::vec3{0, 0, -1});
		rotation = ang;
	}


};




#endif