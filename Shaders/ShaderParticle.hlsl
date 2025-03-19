// ShaderParticle.hlsl

cbuffer constants : register(b0)
{
    matrix MVP;
}

struct VS_INPUT
{
    float3 position : POSITION; // Input position from vertex buffer
    float2 tex : TEXCOORD0;
    float4 data: TEXCOORD1;
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float2 tex : TEXCOORD0;
    float4 data: TEXCOORD1;
    float2 texCoord : TEXCOORD2;
};

PS_INPUT ParticleVS(VS_INPUT input)
{
    PS_INPUT output;
    float scrollX, scrollY;

    output.position = mul(float4(input.position, 1.0f), MVP);
    output.tex = input.tex;
    output.data = input.data;

    scrollX = input.data.y;
    scrollY = input.data.z;

    output.texCoord.x = input.tex.x - scrollX;
    output.texCoord.y = input.tex.y + scrollY;
    
    return output;
}

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

float4 ParticlePS(PS_INPUT input) : SV_TARGET
{
    float4 color;
    float alpha;
    float intensity;
    
    // rgb color 샘플링
    color = shaderTexture.Sample(SampleType, input.tex);

    // 스크롤링된 텍스쳐 좌표를 이용해 alpha 샘플링
    alpha = shaderTexture.Sample(SampleType, input.texCoord).a;

    // rgb에 alpha를 곱해서 파티클의 흐르는 듯한 효과 생성
    color.rgb = color.rgb * alpha;

    // lifeTime을 이용해 intensity로 사용
    // 파티클이 오래될 수록 사라질 것임
    intensity = input.data.x;

    // 점점 사라지는 효과
    color.rgb = color.rgb * intensity;

    color.a = 1.0f;

    return color;
}
