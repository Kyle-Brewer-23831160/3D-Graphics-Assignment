#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

using namespace DirectX;

struct ObjectTransform
{
	float PosX = 0.0f;
	float PosY = -1.5f;
	float PosZ = 0.0f;

	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;

	float Scaler = 1.0f;
};

class Mesh
{
public:
    Mesh(int Row, int Column, int Depth, std::vector<Mesh> &list, ID3D11ShaderResourceView* texture);
	Mesh() = default;
	void CreateWorldMatrix(ObjectTransform& t);
	XMMATRIX ReturnMatrix();
	ID3D11ShaderResourceView* GetTexture() { return pTexture.Get(); }
	ObjectTransform ObjTransform;
private:
	XMMATRIX WorldMatrix;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;
};

