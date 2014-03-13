#include "Model.h"

void Model::LoadContent(std::vector<Face> f, std::vector<Vector3> v, GraphicsDevice* t_graphicsDevice) {
	faces = f;
	vertices = v;
	graphicsDevice = t_graphicsDevice;
}

void Model::Draw() {
	if(!showWire && !showFaces) 
		return;

	std::vector<Vector3> t_verts(vertices);
	
	Matrix allMats = transMat * rotMat * scaleMat;

	for (int i = 0, n = t_verts.size(); i < n; ++i) {
		t_verts[i] = allMats * vertices[i];
	}

	int color = MAKE_COLOR(0, 255, 0, 255);

	for (int i = 0, n = faces.size(); i < n; ++i) {
		if(showFaces)
			graphicsDevice->DrawTriangle(t_verts[faces[i].a1 - 1], t_verts[faces[i].b1 - 1], t_verts[faces[i].c1 - 1]);

		if(showWire) {
			graphicsDevice->DrawLine(t_verts[faces[i].a1 - 1], t_verts[faces[i].b1 - 1], color);
			graphicsDevice->DrawLine(t_verts[faces[i].a1 - 1], t_verts[faces[i].c1 - 1], color);
			graphicsDevice->DrawLine(t_verts[faces[i].b1 - 1], t_verts[faces[i].c1 - 1], color);
		}
		
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
	rotMat = matSum * rotMat;
	//for (int i = 0, n = vertices.size(); i < n; ++i) {
	//	vertices[i] = matSum * vertices[i];
	//}
}

void Model::Translate(Vector3 translation){
	Matrix mat;
	mat.m[3] = translation.x;
	mat.m[7] = translation.y;
	mat.m[11] = translation.z;

	transMat =  mat * transMat;
	//for (int i = 0, n = vertices.size(); i < n; ++i) {
	//	vertices[i] = mat * vertices[i];
	//}
}

void Model::Scale(Vector3 scale){
	Matrix mat;
	mat.m[0] = scale.x;
	mat.m[5] = scale.y;
	mat.m[10] = scale.z;

	scaleMat = mat * scaleMat;
	//for (int i = 0, n = vertices.size(); i < n; ++i) {
	//	vertices[i] = mat * vertices[i];
	//}
}