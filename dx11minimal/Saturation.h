Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float3 RGBtoHSL(float3 rgb) {
    float minVal = min(min(rgb.r, rgb.g), rgb.b);
    float maxVal = max(max(rgb.r, rgb.g), rgb.b);
    float delta = maxVal - minVal;

    float l = (maxVal + minVal) / 2.0;

    float s = delta / (1 - abs(2 * l - 1) + 1e-10);
    s = lerp(0, s, step(1e-5, delta));

    float3 rgbDelta = (maxVal - rgb) / (delta + 1e-10);
    float3 selector = step(maxVal.xxx, rgb);

    float h = dot(selector, float3(
        rgbDelta.b - rgbDelta.g,
        rgbDelta.r - rgbDelta.b + 2,
        rgbDelta.g - rgbDelta.r + 4
    ));
    h = fmod(h / 6 + 1, 1);

    return float3(h, s, l);
}

float3 HSLtoRGB(float3 hsl) {
    float h = hsl.x * 2;
    float c = (1 - abs(2 * hsl.z - 1)) * hsl.y;
    float x = c * (1 - abs(fmod(h, 2) - 1));
    float m = hsl.z - c * 0.5;

    float3 isSector0to1 = step(0, h) * (1 - step(1, h));
    float3 isSector1to2 = step(1, h) * (1 - step(2, h));
    float3 isSector2to3 = step(2, h) * (1 - step(3, h));
    float3 isSector3to4 = step(3, h) * (1 - step(4, h));
    float3 isSector4to5 = step(4, h) * (1 - step(5, h));
    float3 isSector5to6 = step(5, h) * (1 - step(6, h));

    float3 rgb =
        isSector0to1 * float3(c, x, 0) +
        isSector1to2 * float3(x, c, 0) +
        isSector2to3 * float3(0, c, x) +
        isSector3to4 * float3(0, x, c) +
        isSector4to5 * float3(x, 0, c) +
        isSector5to6 * float3(c, 0, x);

    return saturate(rgb + m);
}

static const float saturation = 1;

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 color = inputTexture.Sample(samplerState, input.uv).rgb;
    float3 hsl = RGBtoHSL(color);

    // Меняем насыщенность (saturation = hsl.y)
    hsl.y *= saturation; // saturation можно задавать извне (0..2)

    float3 result = HSLtoRGB(hsl);
    return float4(result, 1.0);
}