#pragma once
#include <DirectXMath.h>
#include "Mesh.h"

class PlayerCamera
{
public:
	PlayerCamera();
	void CreateCamMatrix(ObjectTransform& t);
	PlayerCamera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target);
	void SetPositions(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target);
	DirectX::XMMATRIX GetViewMatrix();
private:
	//DirectX::XMFLOAT3 position;
	//DirectX::XMFLOAT3 mTarget;
	//DirectX::XMFLOAT3 up;
	ObjectTransform CameraTransform;
	XMMATRIX CamMatrix;
};

