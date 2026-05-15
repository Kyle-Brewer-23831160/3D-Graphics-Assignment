cbuffer ConstantBuffer : register(b0)
{
    matrix wvp;
    float3 lightColour;
    float ambientIntensity;
    
    matrix world;
    matrix normalMatrix;
    float3 lightDirection;
    float DiffuseIntensity;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNorm : NORMAL;
};

Texture2D texObject : register(t0);
SamplerState objSampleState : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{ 
    float4 baseColor = texObject.Sample(objSampleState, input.inTexCoord);
    float3 normal = normalize(input.inNorm);
    float3 lightDir = normalize(lightDirection);
    
    //ambient
    float3 ambient = lightColour * ambientIntensity;
    
    //diffuse
    float3 DiffuseFactor = max(dot(normal, lightDir), 0.0f);
    float3 Diffuse = lightColour * DiffuseIntensity * DiffuseFactor;
    
    float3 lighting = ambient + Diffuse;
    
    float4 finalColor = float4(baseColor.rgb * lighting, baseColor.a);
    
    return finalColor;
}