#ifndef VECTOR2_H
#define VECTOR2_H


struct Vec2 {
public:

	float x{};
	float y{};

	Vec2 normalize();
	Vec2 scaleBy(float scalar);
	Vec2 rotateBy(float rad);
	Vec2 translateBy(const Vec2& other);
	Vec2 translateBy(float dx, float dy);

	float magnitude() const;
	float getAngle() const;
	float distTo(const Vec2& other) const;
	float distToSq(const Vec2& other) const;
	float dot(const Vec2& other) const;

	void operator+=(const Vec2& other);
	Vec2 operator*(float scalar);

};






#endif