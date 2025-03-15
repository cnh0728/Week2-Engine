// ShaderFont.hlsl

cbuffer constants : register(b3)
{
    matrix MVP;
}

cbuffer PixelBuffer : register(b4)
{
    float4 pixelColor;
};

struct VS_INPUT
{
    float4 position : POSITION; // Input position from vertex buffer
    float2 texCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float2 texCoord : TEXCOORD0;
};

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

PS_INPUT FontVS(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(float4(input.position.xyz, 1.0f), MVP);
    output.texCoord = input.texCoord;
    
    return output;
}


float4 FontPS(PS_INPUT input) : SV_TARGET
{
    float4 color;
    
    color = shaderTexture.Sample(SampleType, input.texCoord);
    
    // 텍스처에서 색상이 검정색인 경우 이 픽셀을 투명하게 처리합니다.
    
    if (color.r == 0.0f)
    {
        color.a = 0.0f;
    }
    // 텍스처에서 색상이 검정색이 아닌 경우, 이는 폰트의 픽셀이므로 폰트 픽셀 색상을 사용하여 그립니다.
    else
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }

    return color;
}
