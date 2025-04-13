cbuffer global : register(b5)
{
    float4 gConst[32];
};

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
};

cbuffer camera : register(b3)
{
    float4x4 world[2];
    float4x4 view[2];
    float4x4 proj[2];
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

cbuffer gc : register(b0)
{
    float gx;
    float gy;
};


struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

float3 rotY(float3 pos, float a)
{
    float3x3 m =
    {
        cos(a), 0, sin(a),
        0, 1, 0,
        -sin(a), 0, cos(a)
    };
    pos = mul(pos, m);
    return pos;
}

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    float2 quad[6] = { -1, -1, 1, -1, -1, 1, 1, -1, 1, 1, -1, 1 };
    float2 p = quad[vID % 6];
    uint n = vID / 6;

    uint offsetX = (n % (int)gx) * 2;
    uint offsetY = (n / (int)gx) * 2;
    p.x += offsetX - (gx - 1);
    p.y += offsetY - (gy - 1);

    float r = 1;
    float r2 = 2;
    float knots = 2;
    p.x = (p.x / (gx - 0.5)) * 3.14;
    p.y = (p.y / (gy - 0.5)) * 3.14;

    //float4 pos = float4(p.x, p.y, r * cos(p.x / 2), 1);
    float4 pos = float4(0, 0, 0, 1);

    /*pos.x = sin(p.x) * (r2 + cos(p.y) * r);
    pos.y = cos(p.x) * (r2 + cos(p.y) * r);
    pos.z = sin(p.y) * r;*/

    pos.x = cos(p.x) * r * (r2 + cos(p.y));
    pos.y = sin(p.x) * r * (r2 + cos(p.y));
    pos.z = sin(p.y) * r;
    
    //pos.xyz *= 0.4;
    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = float2(1, -1) * p / 2. + .5;
    return output;
}
