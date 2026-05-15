#include "Lighting.h"

Lighting::Lighting()
{
	mColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	mAmbientIntensity = 1.0f;
}
Lighting::Lighting(DirectX::XMFLOAT3 color, float ambientIntensity, DirectX::XMFLOAT3 Dir, float diffuseIntensity)
{
	mColor = color;
	mAmbientIntensity = ambientIntensity;
}
DirectX::XMFLOAT3 Lighting::GetColor() {
	return mColor;
}
float Lighting::GetAmbientIntensity() {
	return mAmbientIntensity;
}

DirectX::XMFLOAT3 Lighting::GetDirection()
{
	return mDir;
}

float Lighting::GetDiffuseIntensity()
{
	return mDiffuseIntensity;
}

float Lighting::SetAmbientIntensity(float newIntensity)
{
	return mAmbientIntensity = newIntensity;
}
