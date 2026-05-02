struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D texObject : register(t0);
SamplerState objSampleState : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 pixelcolour = texObject.Sample(objSampleState, input.inTexCoord).rgb;
    return float4(pixelcolour, 1.0f);
}