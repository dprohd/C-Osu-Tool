#pragma once

#include <ostream>

class vector2
{
public:
	float x, y;

	vector2();
	vector2(float x, float y);

	float length() const;
	float distance_to(const vector2& v) const;

	vector2 operator-(const vector2& v) const;
	vector2 operator+(const vector2& v) const;
	vector2 operator*(const vector2& v) const;
	vector2 operator/(const vector2& v) const;
	vector2& operator=(const vector2& v);
	friend std::ostream& operator<<(std::ostream& os, const vector2& v);
};
