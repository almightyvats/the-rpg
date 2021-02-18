#include "Vector2D.hpp"
#include <math.h>

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}
Vector2D &Vector2D::Subtract(const Vector2D &vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}
Vector2D &Vector2D::Multiply(const Vector2D &vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}
Vector2D &Vector2D::Divide(const Vector2D &vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
	return v1.Add(v2);
}
Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
	return v1.Subtract(v2);
}
Vector2D &operator*(Vector2D &v1, const Vector2D &v2)
{
	return v1.Multiply(v2);
}
Vector2D &operator/(Vector2D &v1, const Vector2D &v2)
{
	return v1.Divide(v2);
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
	return this->Add(vec);
}
Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
	return this->Subtract(vec);
}
Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
	return this->Multiply(vec);
}
Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
	return this->Divide(vec);
}

Vector2D &Vector2D::operator*(const int &i)
{
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2D &Vector2D::operator*(const float &f)
{
	this->x *= f;
	this->y *= f;

	return *this;
}

Vector2D &Vector2D::Zero()
{
	this->x = 0.0f;
	this->y = 0.0f;

	return *this;
}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}

float Vector2D::Distance(const Vector2D &vec1, const Vector2D &vec2)
{
	return sqrt(pow((vec1.x - vec2.x), 2) + pow((vec1.y - vec2.y), 2));
}

float Vector2D::Length(const Vector2D &vec)
{
	if (vec.x == 0.0f && vec.y == 0.0f) {
		return 0.0f;
	}
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

Vector2D Vector2D::Normalize(const Vector2D &vec)
{
	float length = Vector2D::Length(vec);
	return Vector2D(vec.x * (1.0f / length), vec.y * (1.0f / length));
}