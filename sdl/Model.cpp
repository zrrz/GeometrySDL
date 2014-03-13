#include "Model.h"

void Model::LoadContent(std::vector<Face> f, std::vector<Vector3> v, GraphicsDevice* t_graphicsDevice) {
	faces = f;
	vertices = v;
	graphicsDevice = t_graphicsDevice;
}

void Model::Draw() {
	for (int i = 0, n = faces.size(); i < n; ++i) {

		//graphicsDevice->DrawLine(vertices[faces[i].a1 - 1], vertices[faces[i].b1 - 1]);
		//graphicsDevice->DrawLine(vertices[faces[i].a1 - 1], vertices[faces[i].c1 - 1]);
		//graphicsDevice->DrawLine(vertices[faces[i].b1 - 1], vertices[faces[i].c1 - 1]);

		graphicsDevice->DrawTriangle(vertices[faces[i].a1 - 1], vertices[faces[i].b1 - 1], vertices[faces[i].c1 - 1]);
	}
}

void Model::Rotate(Vector3 rotation){
	Matrix matX, matY, matZ;
	if (abs(rotation.x) > 0.0f) {
		float theta = rotation.x * (PI/180.0f);
		matX.m[5] = cos(theta);
		matX.m[6] = -sin(theta);
		matX.m[9] = sin(theta);
		matX.m[10] = cos(theta);
	}
	if (abs(rotation.y) > 0.0f) {
		float theta = rotation.y * (PI / 180.0f);
		matY.m[0] = cos(theta);
		matY.m[2] = sin(theta);
		matY.m[8] = -sin(theta);
		matY.m[10] = cos(theta);
	}
	if (abs(rotation.z) > 0.0f) {
		float theta = rotation.z * (PI / 180.0f);
		matZ.m[0] = cos(theta);
		matZ.m[1] = -sin(theta);
		matZ.m[4] = sin(theta);
		matZ.m[5] = cos(theta);
	}
	Matrix matSum = matX*matY*matZ;
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] = matSum * vertices[i];
	}
}

void Model::Translate(Vector3 translation){
	Matrix mat;
	mat.m[3] = translation.x;
	mat.m[7] = translation.y;
	mat.m[11] = translation.z;

	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] = mat * vertices[i];
	}
}

void Model::Scale(Vector3 scale){
	Matrix mat;
	mat.m[0] = scale.x;
	mat.m[5] = scale.y;
	mat.m[10] = scale.z;

	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] = mat * vertices[i];
	}
}