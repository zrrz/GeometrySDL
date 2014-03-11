#include "Vectors.h"

Vector3::Vector3() {
	x = y = z = 0.0f;
	w = 1.0f;
}

Vector3::Vector3(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
	w = 1;
}

Vector3::Vector3(const Vector3& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

Vector3::~Vector3() {

}

Vector3 Vector3::operator+(const Vector3& other) const {
	Vector3 result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;
	result.w = 1;
	return result;
}

Vector3 Vector3::operator-(const Vector3& other) const {
	Vector3 result;
	result.x = x - other.x;
	result.y = y - other.y;
	result.z = z - other.z;
	result.w = 1;
	return result;
}

Vector3 Vector3::operator*(float c) const {
	Vector3 result;
	result.x = x*c;
	result.y = y*c;
	result.z = z*c;
	result.w = 1;
	return result;
}

Vector3 operator*(float c, const Vector3& v) {
	return v*c;
	/*Vector3 result;
	result.x = v.x*c;
	result.y = v.y*c;
	result.z = v.z*c;
	result.w = 1;
	return result;*/
}

float Vector3::operator*(const Vector3& other) const {
	return x*other.x + y*other.y + z*other.z + w*other.w;
}

Vector3 Vector3::operator=(const Vector3& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

	return *this;
}

Vector3 Vector3::operator=(const Vector2& other) {
	x = other.x;
	y = other.y;
	z = 1;
	w = 1;

	return *this;
}