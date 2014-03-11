#ifndef _MODEL_H_
#define _MODEL_H_

#include "Vectors.h"
#include "Face.h"
#include <vector>
#include "Matrix.h"
#include "GraphicsDevice.h"

class Model {
public:
	Model(){};
	void LoadContent(std::vector<Face> f, std::vector<Vector3> v, GraphicsDevice* t_graphicsDevice);
	void Draw();
	void Rotate(Vector3);
	void Translate(Vector3);
	void Scale(Vector3);
private:
	GraphicsDevice *graphicsDevice;
	std::vector<Face> faces;
	std::vector<Vector3> vertices;
};

#endif