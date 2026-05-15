cbuffer ConstantBuffer : register(b0)
{
    matrix WVP;
    float3 lightColour;
    float ambientIntensity;
    
    matrix world;
    matrix normalMatrix;
    float3 lightDirection;
    float DiffuseIntensity;
}

struct vertexIn
{
    float3 position : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNorm : NORMAL;
};

struct vertexOut
{
    float4 position : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNorm : NORMAL;
};

vertexOut main(vertexIn input)
{
    vertexOut output;
    output.position = mul(float4(input.position, 1.0f), WVP);
    output.outTexCoord = input.inTexCoord;
    output.outNorm = mul(float4(input.inNorm, 0.0f), normalMatrix).xyz;
    return output;
}