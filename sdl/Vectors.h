#ifndef _VECTORS_H_
#define _VECTORS_H_

#include <iostream>

#define PI 3.141592653

struct Vector2
{
	float x, y;
};

class Vector3 {
public:
	Vector3();
	Vector3(float, float, float);
	Vector3(const Vector3& other);
	~Vector3();

	// add/subtract
	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;

	// scalar multiplication
	Vector3 operator*(float c) const;
	friend Vector3 operator*(float c, const Vector3& v);

	// dot product
	float operator*(const Vector3& other) const;

	// assingment
	Vector3 operator=(const Vector3& other);
	Vector3 operator=(const Vector2& other);

	float x, y, z, w;
 
};
#endif