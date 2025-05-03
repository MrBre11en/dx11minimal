struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

static const float2 directions[4] =
{
    float2(1, 1),
    float2(-1, 1),
    float2(1, -1),
    float2(-1, -1)
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    uint width, height;
    float2 texelSize = float2(1, 1) / float2(width, height);

    int iterations = 4;
    float offsetScale = 0.002;

    float4 color = float4(0, 0, 0, 0);
    for (int i = 0; i < iterations; i++)
    {
        float offset = offsetScale * (i + 0.5);
        for (int j = 0; j < 4; j++)
        {
            float2 uv = input.uv + directions[j] * offset;
            color += inputTexture.Sample(samplerState, uv);
        }
    }

    color /= (iterations * 4);
    return color;
}