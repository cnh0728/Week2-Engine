// ShaderFont.hlsl
cbuffer constants : register(b0)
{
    matrix MVP;
}

cbuffer PixelBuffer : register(b1)
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

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

Texture2D shaderTexture;
SamplerState SampleType;

PS_INPUT FontVS(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(input.position, MVP);
    output.texCoord = input.texCoord;
    
    return output;
}


PS_OUTPUT FontPS(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    output.color = shaderTexture.Sample(SampleType, input.texCoord);
    
    // 텍스처에서 색상이 검정색인 경우 이 픽셀을 투명하게 처리합니다.
    
    if (output.color.r == 0.0f)
    {
        output.color.a = 0.0f;
    }
    // 텍스처에서 색상이 검정색이 아닌 경우, 이는 폰트의 픽셀이므로 폰트 픽셀 색상을 사용하여 그립니다.
    else
    {
        output.color.a = 1.0f;
        output.color = output.color * pixelColor;
    }

    return output;
}
