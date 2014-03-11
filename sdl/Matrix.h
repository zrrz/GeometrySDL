#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vectors.h"

class Matrix {
public:
	Matrix();
	Matrix(float[]);
	Matrix(const Matrix& other);

	Vector3 operator*(const Vector3& v) const;
	Matrix operator*(const Matrix& o) const;
	Matrix operator*(const float& f) const;

	Matrix operator=(float[]);
	Matrix operator=(const Matrix& o);

	Matrix Inverse();
	Matrix Transpose();

	float m[16];
};

#endif