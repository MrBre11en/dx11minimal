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
    float h = 0, s = 0, l = (maxVal + minVal) / 2.0;

    if (delta != 0) {
        s = l < 0.5 ? delta / (maxVal + minVal) : delta / (2.0 - maxVal - minVal);

        if (rgb.r == maxVal) h = (rgb.g - rgb.b) / delta + (rgb.g < rgb.b ? 6.0 : 0.0);
        else if (rgb.g == maxVal) h = (rgb.b - rgb.r) / delta + 2.0;
        else h = (rgb.r - rgb.g) / delta + 4.0;

        h /= 6.0;
    }
    return float3(h, s, l);
}

float3 HSLtoRGB(float3 hsl) {
    float h = hsl.x, s = hsl.y, l = hsl.z;
    float c = (1 - abs(2 * l - 1)) * s;
    float x = c * (1 - abs(fmod(h * 6, 2) - 1));
    float m = l - c / 2;
    float3 rgb;

    if (h < 1.0 / 6.0)      rgb = float3(c, x, 0);
    else if (h < 2.0 / 6.0) rgb = float3(x, c, 0);
    else if (h < 3.0 / 6.0) rgb = float3(0, c, x);
    else if (h < 4.0 / 6.0) rgb = float3(0, x, c);
    else if (h < 5.0 / 6.0) rgb = float3(x, 0, c);
    else                  rgb = float3(c, 0, x);

    return rgb + m;
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