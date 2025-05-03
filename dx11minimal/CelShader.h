cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
    float2 iResolution;
    float2 pad;
};

Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float rand(float2 uv, float time)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233)) * 43758.5453 * time));
}

float noise(float2 uv, float time)
{
    float2 i = floor(uv);
    float2 f = frac(uv);
    f = f * f * (3 - 2 * f);
    uv = i + f;
    return -1 + 2 * rand(uv, time);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float noiseIntensity = 0.1;
    float scanlineSpeed = 5;
    float ntime = time.x;

    // Исходный цвет
    float4 color = inputTexture.Sample(samplerState, input.uv);

    // Цветовой шум
    float2 offsetR = float2(0.01, 0.0) * noise(input.uv * 10, ntime);
    float2 offsetB = float2(-0.01, 0.0) * noise(input.uv * 10 + 0.5, ntime);
    color.r = inputTexture.Sample(samplerState, input.uv + offsetR).r;
    color.b = inputTexture.Sample(samplerState, input.uv + offsetR).b;

    // Горизонтальные полосы
    float scanline = sin(input.uv.y * 500 + ntime * scanlineSpeed) * 0.1;
    color.rgb += scanline;

    // Мерцание
    float flicker = 1 + (rand(float2(ntime, ntime), ntime) - 0.5) * 0.2;
    color.rgb *= flicker;

    // Черно-белый шум
    float grain = rand(input.uv, ntime) * noiseIntensity;
    color.rgb += grain;

    return saturate(color);
}