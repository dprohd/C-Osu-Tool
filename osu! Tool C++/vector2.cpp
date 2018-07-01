#include "vector2.h"

vector2::vector2()
{
}

vector2::vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float vector2::length() const
{
	return std::sqrt(x * x + y * y);
}

float vector2::distance_to(const vector2& v) const
{
	const vector2 delta = v - *this;
	return delta.length();
}

vector2 vector2::operator-(const vector2& v) const
{
	return vector2(x - v.x, y - v.y);
}

vector2 vector2::operator+(const vector2& v) const
{
	return vector2(x + v.x, y + v.y);
}

vector2 vector2::operator*(const vector2& v) const
{
	return vector2(x * v.x, y * v.y);
}

vector2 vector2::operator/(const vector2& v) const
{
	return vector2(x / v.x, y / v.y);
}

vector2& vector2::operator=(const vector2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const vector2& v)
{
	os << "X: " << v.x << ", Y: " << v.y;
	return os;
}
