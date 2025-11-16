#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

#include <iostream>

using vec3 = glm::vec3;

class Camera {
private:
	//fps camera
	float lastX{};
	float lastY{};
	bool firstMouse = true;
public:
	vec3 pos{ 0.0f, 0.0f, 3.0f };
	vec3 front{ 0.0f, 0.0f, -1.0f };
	vec3 up{ 0.0f, 1.0f, 0.0f };

	vec3 eularAngles{ 0.0f, -90.0f, 0.0f };
	float& pitch{ eularAngles.x };
	float& yaw{ eularAngles.y };
	float& roll{ eularAngles.z };

	//fps camera
	float sensitivity{0.1f};
	
	glm::mat4 getView() const {
		return glm::lookAt(pos, pos + front, up);
	}

	void setEularAngles(float pitch, float yaw, float roll) {
		eularAngles = { pitch, yaw, roll };
		front.x = std::cosf(glm::radians(yaw)) * std::cosf(glm::radians(pitch));
		front.y = std::sinf(glm::radians(pitch));
		front.z = std::sinf(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);
	}

	void updateFPSCamera(float dx, float dy) {
		/*if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}
		float dx = (x - lastX) * sensitivity;
		float dy = (y - lastY) * sensitivity;*/

		dx *= sensitivity;
		dy *= -sensitivity;
		
		setEularAngles(std::clamp(pitch + dy, -89.0f, 89.0f), yaw + dx, 0.0f);

	}

};




#endif