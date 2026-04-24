#include "Mesh.h"

Mesh::Mesh(int Row, int Column, int Depth, std::vector<Mesh> &list, ID3D11ShaderResourceView* texture)
{
	pTexture = texture;
	ObjTransform.PosX += Row * 1.003f;
	ObjTransform.PosY += Column * 1.003f;
	ObjTransform.PosZ += Depth * 1.003f;
	CreateWorldMatrix(ObjTransform);

	list.push_back(*this);
}

void Mesh::CreateWorldMatrix(ObjectTransform& t)
{
	XMMATRIX translation = XMMatrixTranslation(t.PosX, t.PosY, t.PosZ);
	XMMATRIX rotY = XMMatrixRotationY(t.RotY);
	XMMATRIX rotX = XMMatrixRotationX(t.RotX);
	XMMATRIX Scale = XMMatrixScaling(t.Scaler, t.Scaler, t.Scaler); //uniform scaling

	WorldMatrix = Scale * rotX * rotY * translation;
}

XMMATRIX Mesh::ReturnMatrix()
{
	return WorldMatrix;
}
