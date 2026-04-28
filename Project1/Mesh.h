#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include<string>
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

	float Scaler = 3.0f;
};

struct OBB
{
	XMFLOAT3 Center;
	XMFLOAT3 Axis[3]; 
	XMFLOAT3 Extents;
};

class Mesh
{
public:
    Mesh(int Row, int Column, int Depth, ID3D11ShaderResourceView* texture);
	Mesh() = default;
	void CreateWorldMatrix(ObjectTransform t);
	void Move(float forward, float strafe, float deltaTime, float Yaw);
	XMMATRIX ReturnMatrix();
	ID3D11ShaderResourceView* GetTexture() { return pTexture.Get(); }
	ObjectTransform ObjTransform;
	int TileIndex;
private:
	XMMATRIX WorldMatrix;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;
};

