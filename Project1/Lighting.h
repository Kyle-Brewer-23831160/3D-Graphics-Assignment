#pragma once
#include <DirectXMath.h>

class Lighting
{
public:
	Lighting(); // Default: white light, full intensity
	Lighting(DirectX::XMFLOAT3 color, float ambientIntensity, DirectX::XMFLOAT3 Dir, float diffuseIntensity);

	DirectX::XMFLOAT3 GetColor();
	float GetAmbientIntensity();

	DirectX::XMFLOAT3 GetDirection();
	float GetDiffuseIntensity();

	float SetAmbientIntensity(float newIntensity);
private:
	DirectX::XMFLOAT3 mColor;
	float mAmbientIntensity;

	DirectX::XMFLOAT3 mDir;
	float mDiffuseIntensity;
};

