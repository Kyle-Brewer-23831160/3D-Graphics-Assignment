cbuffer ConstantBuffer : register(b0)
{
    matrix WVP;
}

struct vertexIn
{
    float3 position : POSITION;
    float2 inTexCoord : TEXCOORD;
};

struct vertexOut
{
    float4 position : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

vertexOut main(vertexIn input)
{
    vertexOut output;
    output.position = mul(float4(input.position, 1.0f), WVP);
    output.outTexCoord = input.inTexCoord;
    return output;
}