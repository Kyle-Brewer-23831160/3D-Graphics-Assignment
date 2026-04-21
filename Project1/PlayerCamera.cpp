#include "PlayerCamera.h"

using namespace DirectX;

PlayerCamera::PlayerCamera()
{
	CameraTransform.PosX = 0;
	CameraTransform.PosY = 0;
	CameraTransform.PosZ = 0;
	 GetViewMatrix();
}

void PlayerCamera::CreateCamMatrix(ObjectTransform& t)
{
	XMMATRIX translation = XMMatrixTranslation(t.PosX, t.PosY, t.PosZ);
	XMMATRIX rotY = XMMatrixRotationY(t.RotY);
	XMMATRIX rotX = XMMatrixRotationX(t.RotX);
	XMMATRIX Scale = XMMatrixScaling(t.Scaler, t.Scaler, t.Scaler); //uniform scaling

	CamMatrix = Scale * rotX * rotY * translation;
}

DirectX::XMMATRIX PlayerCamera::GetViewMatrix()
{
	
	return DirectX::XMMATRIX();
}
