struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    float2 quad[6] = {
        float2(-1, -1), float2(1, -1), float2(-1, 1),
        float2(1, -1), float2(1, 1), float2(-1, 1)
    };

    float2 p = quad[vID % 6];
    float2 pos = quad[p];

    float2 uv = float2(px + 0.5 + p.x * 0.5, py + 0.5 + p.y * 0.5) / float2(gx, gy);

    output.pos = mul(float4(pos, 1.0), mul(view[0], proj[0]));
    output.uv = uv;
    return output;
}