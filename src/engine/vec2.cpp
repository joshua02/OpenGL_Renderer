#include "vec2.h"
#include <cmath>


float Vec2::magnitude() const {
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

float Vec2::distTo(const Vec2& other) const {
	return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
}

float Vec2::distToSq(const Vec2& other) const {
	return std::pow(other.x - x, 2) + std::pow(other.y - y, 2);
}

float Vec2::getAngle() const {
	return std::atan2(y, x);
}

float Vec2::dot(const Vec2& other) const {
	return x * other.x + y * other.y;
}

Vec2 Vec2::normalize() {

	float mag{ magnitude() };
	if (mag == 0) {
		return *this;
	}

	x /= mag;
	y /= mag;
	return *this;
}

Vec2 Vec2::scaleBy(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2 Vec2::rotateBy(float rad) {

	float newAng{ getAngle() + rad };
	float mag{ magnitude() };

	x = mag * std::cos(newAng);
	y = mag * std::sin(newAng);

	return *this;
}

Vec2 Vec2::translateBy(const Vec2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vec2 Vec2::translateBy(float dx, float dy) {
	x += dx;
	y += dy;
	return *this;
}

void Vec2::operator+=(const Vec2& other) {
	x += other.x;
	y += other.y;
}

Vec2 Vec2::operator*(float scalar) {
	return Vec2{ x * scalar, y * scalar };
}