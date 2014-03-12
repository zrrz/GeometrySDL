#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vectors.h"
#include "Matrix.h"
#include <math.h>

class Camera {

public:
	Camera() {
		Vector3 zero;
		position = zero;
		Vector3 forward(0.0f, 0.0f, 1.0f);
		lookAt = forward;
		fov = 45.0f * (180.0f / PI);
		UpdateMatrix();
	}

	void UpdateMatrix() {

		Matrix persectiveProj;
		persectiveProj.m[10] = (s_nearClip + s_farClip) / s_nearClip;
		persectiveProj.m[11] = -s_farClip;
		persectiveProj.m[14] = 1.0f / s_nearClip;
		persectiveProj.m[15] = 0;

		Vector3 foo(20.0f, 30.0f, 40.0f);
		

		foo = persectiveProj * foo;
		foo = foo / foo.w;

		Vector3 up(0.0f, 1.0f, 0.0f);
		//viewMat = Matrix.CreateLookAt(pos, lookAt, up);
		matrix = viewMat.Inverse();

		//aspectRatio = 640 / 480 //may need swapping
		//projMat = CreatePerspectiveFildOfView(fov, aspectRatio, s_nearClip, s_farClip);
	}
private:

	const float s_nearClip = 1.0f;
	const float s_farClip = 10000.0f;

	Vector3 position;
	Vector3 lookAt;
	Matrix matrix;
	Matrix viewMat;
	Matrix projMat;

	float fov;
};

#endif