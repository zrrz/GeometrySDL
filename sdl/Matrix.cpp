
#include "Matrix.h"

Matrix::Matrix() {
	m[0] = m[5] = m[10] = m[15] = 1;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0;
}

Matrix::Matrix(float arr[]) {
	memcpy(m, arr, sizeof(float)*16);
}

Matrix::Matrix(const Matrix& o) {
	memcpy(m, o.m, sizeof(float)*16);
}

Matrix Matrix::operator=(float arr[]) {
	memcpy(m, arr, sizeof(float)*16);
	return m;
}

Matrix Matrix::operator=(const Matrix& o) {
	memcpy(m, o.m, sizeof(float)*16);
	return m;
}

Vector3 Matrix::operator*(const Vector3& v) const {
	Vector3 result;

	result.x = m[0]*v.x + m[1]*v.y + m[2]*v.z + m[3]*v.w;
	result.y = m[4]*v.x + m[5]*v.y + m[6]*v.z + m[7]*v.w;
	result.z = m[8]*v.x + m[9]*v.y + m[10]*v.z + m[11]*v.w;
	result.w = m[12]*v.x + m[13]*v.y + m[14]*v.z + m[15]*v.w;

	return result;
}

Matrix Matrix::operator*(const Matrix& o) const {
	Vector3 row[4];
	Vector3 col[4];

	Matrix result;

	for(int i = 0; i < 4; i++) {
		row[i].x = m[4*i];
		row[i].y = m[4*i + 1];
		row[i].z = m[4*i + 2];
		row[i].w = m[4*i + 3];

		col[i].x = o.m[0 + i];
		col[i].y = o.m[4 + i];
		col[i].z = o.m[8 + i];
		col[i].w = o.m[12 + i];
	}

	int i = 0;
	for(int r = 0; r < 4; r++) {
		for(int c = 0; c < 4; c++) {
			result.m[i] = row[r]*col[c];
			i++;
		}
	}

	return result;
}

Matrix Matrix::operator*(const float& f) const {
	Matrix result;
	for (int i = 0; i < 16; ++i) {
		result.m[i] = m[i] * f;
	}
	return result;
}

Matrix Matrix::Inverse() {
	Matrix result;

	//|ax ay az|   (aybz - azby)cx
	//|bx by bz| = + (azbx - axbz)cy = (a x b) * c
	//|cx cy cz|   + (axby - aybx)cz

	//|0  1  2  3 |
	//|4  5  6  7 |
	//|8  9  10 11|
	//|12 13 14 15|

	float determinant = (((m[1] * m[6]) - (m[2] * m[5])) * m[8]) + (((m[2] * m[4]) - (m[0] * m[6])) * m[9]) + (((m[0] * m[5]) - (m[1] * m[4])) * m[10]);

	Matrix adjoint;
	adjoint.m[0] = m[5] * m[10] - m[6] * m[9];
	adjoint.m[1] = -(m[4] * m[10] - m[6] * m[8]);
	adjoint.m[2] = m[4] * m[8] - m[5] * m[8];
	adjoint.m[3] = -(m[1] * m[10] - m[2] * m[9]);
	adjoint.m[4] = m[0] * m[10] - m[8] * m[2];
	adjoint.m[5] = -(m[0] * m[9] - m[1] * m[8]);
	adjoint.m[6] = m[1] * m[6] - m[2] * m[5];
	adjoint.m[7] = -(m[0] * m[6] - m[2] * m[4]);
	adjoint.m[8] = m[0] * m[5] - m[1] * m[4];

	result = adjoint * (1.0f / determinant);
	return result.Transpose();
}
Matrix Matrix::Transpose() {
	Matrix result;

	result.m[1] = m[4];
	result.m[2] = m[8];
	result.m[4] = m[1];
	result.m[6] = m[9];
	result.m[8] = m[2];
	result.m[9] = m[6];

	return result;
}