// ShaderW0.hlsl
cbuffer constants : register(b0)
{
    matrix MVP;
    // float4 CustomColor;
    // uint bUseVertexColor;
}

cbuffer UUIDColor : register(b1){
    float4 UUIDColor;
}

cbuffer Depth : register(b2){
    int depth;
    int nearPlane;
    int farPlane;
}

struct VS_INPUT
{
    float4 position : POSITION; // Input position from vertex buffer
    float4 color : COLOR;       // Input color from vertex buffer

    // matrix MVP;
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR;          // Color to pass to the pixel shader
    float3 normal : NORMAL;
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

    
    output.position = mul(input.position, MVP);
    // output.depthPosition = output.position;
    // output.position = input.position;
    
    // output.color = bUseVertexColor == 1 ? input.color : CustomColor;
    output.color = input.color;
    return output;
}


PS_OUTPUT mainPS(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT output;

    // 기본 깊이 값 계산 (0.0~1.0)
    // float baseDepth = input.depthPosition.z / input.depthPosition.w;

    // 색상 설정 (예: 흰색)
    output.color = input.color;
    // output.depth = saturate(depth);
    // output.color = float4(depth, depth, depth, 1.0f);
    
    return output;
}

float4 outlinePS(PS_INPUT input) : SV_TARGET
{
    // Output the color directly
    return float4(1.0f, 0.647f, 0.0f, 0.1f);
}

PS_OUTPUT PickingPS(PS_INPUT input):SV_TARGET{
    PS_OUTPUT output;
    output.color = UUIDColor;
    return output;
}