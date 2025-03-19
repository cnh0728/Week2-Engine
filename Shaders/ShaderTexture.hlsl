// ShaderTexture.hlsl

cbuffer constants : register(b0)
{
    matrix MVP;
}

struct VS_INPUT
{
    float3 position : POSITION; // Input position from vertex buffer
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float2 texCoord : TEXCOORD0;
};

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

PS_INPUT TextureVS(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(float4(input.position, 1.0f), MVP);
    output.texCoord = input.texCoord;
    
    return output;
}


float4 TexturePS(PS_INPUT input) : SV_TARGET
{
    float4 color = shaderTexture.Sample(SampleType, input.texCoord);
    
    return color;
}
