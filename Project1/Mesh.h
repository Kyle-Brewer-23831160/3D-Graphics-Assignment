#pragma once

#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

using namespace DirectX;

struct ObjectTransform
{
	float PosX = 0.0f;
	float PosY = 0.0f;
	float PosZ = 0.0f;

	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;

	float Scaler = 1.0f;
};

class Mesh
{
public:
    Mesh(int Row, int Column, int Depth, std::vector<Mesh> &list);
	Mesh() = default;
	void CreateWorldMatrix(ObjectTransform& t);
	XMMATRIX ReturnMatrix();

	ObjectTransform ObjTransform;
private:
	XMMATRIX WorldMatrix;
	/*int Row;
	int Column;
	int Depth;*/
};

