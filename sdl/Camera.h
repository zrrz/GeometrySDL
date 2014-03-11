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