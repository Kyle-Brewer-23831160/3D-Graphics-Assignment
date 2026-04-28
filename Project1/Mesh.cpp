#include "Mesh.h"

Mesh::Mesh(int Row, int Column, int Depth, ID3D11ShaderResourceView* texture)
{
	ObjTransform.PosX = -10;
	ObjTransform.PosY = -30;
    ObjTransform.PosZ = 10;

	pTexture = texture;
	ObjTransform.PosX += Row * 1.003f;
	ObjTransform.PosY += Column * 1.003f;
	ObjTransform.PosZ += Depth * 1.003f;

	CreateWorldMatrix(ObjTransform);
}

void Mesh::CreateWorldMatrix(ObjectTransform t)
{
	XMMATRIX translation = XMMatrixTranslation(t.PosX, t.PosY, t.PosZ);
	XMMATRIX rotY = XMMatrixRotationY(t.RotY);
	XMMATRIX rotX = XMMatrixRotationX(t.RotX);
	XMMATRIX Scale = XMMatrixScaling(t.Scaler, t.Scaler, t.Scaler); //uniform scaling

	WorldMatrix = Scale * rotX * rotY * translation;
}

void Mesh::Move(float forward, float strafe, float deltaTime, float Yaw)
{
	float speed = 10.0f * deltaTime;

	// Move relative to where we are facing
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0, Yaw, 0);
	XMVECTOR forwardVec = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rotation);
	XMVECTOR rightVec = XMVector3TransformCoord(XMVectorSet(1, 0, 0, 0), rotation);

	XMFLOAT3 pos = XMFLOAT3(ObjTransform.PosX, ObjTransform.PosY, ObjTransform.PosZ);
	XMVECTOR posVec = XMLoadFloat3(&pos);
	posVec += forwardVec * forward * speed;
	posVec += rightVec * strafe * speed;
	XMStoreFloat3(&pos, posVec);

	ObjTransform.PosX = pos.x;
	ObjTransform.PosY = pos.y;
	ObjTransform.PosZ = pos.z;
}

XMMATRIX Mesh::ReturnMatrix()
{
	return WorldMatrix;
}
