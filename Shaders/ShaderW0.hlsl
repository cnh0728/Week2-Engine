// ShaderW0.hlsl
Texture2D texDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer constants : register(b0)
{
    matrix MVP;
    float4 CustomColor;
    int PixelType;
}

struct VS_INPUT
{
    float4 position : POSITION; // Input position from vertex buffer
    float3 normal : NORMAL;
    float4 color : COLOR;       // Input color from vertex buffer
    float2 uv : UV;
    // matrix MVP;
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float3 normal : NORMAL;
    float4 color : COLOR;          // Color to pass to the pixel shader
    float2 uv : UV;
    // float4 depthPosition : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
    float depth : SV_Depth;
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;

    output.color = input.color;
    output.position = mul(input.position, MVP);
    output.normal = input.normal;
    output.uv = input.uv;

    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    float4 color;

    color = input.color;
    
    
    switch (PixelType)
    {
    case 1: //CustomColor
        color = CustomColor;
        break;
    case 2: //Texture
        color = texDiffuse.Sample(samLinear, input.uv);
        float avg = (color.r + color.g + color.b) / 3.0f;
        clip(color.a < 0.01f ? -1 : 1);
        break;
    default:
        break;
    }
    
    return color;
}
