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

float3 calculatePositionOnCurve(float u, float p, float q, float radius) {

    const float cu = cos(u);
    const float su = sin(u);
    const float quOverP = q / p * u;
    const float cs = cos(quOverP);

    float3 position;
    position.x = radius * (2 + cs) * 0.5 * cu;
    position.y = radius * (2 + cs) * su * 0.5;
    position.z = radius * sin(quOverP) * 0.5;

    return position;
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

    float2 uvOut = p.xy/float2(gx,gy)/2;

    float r = 1;
    float r2 = 2;
    float tube = 0.4;
    p.x = (p.x / (gx - 0.5)) * 3.14;
    p.y = (p.y / (gy - 0.5)) * 3.14 * 2;


    //float4 pos = float4(p.x, p.y, r * cos(p.x / 2), 1);
    float4 pos = float4(0, 0, 0, 1);

    /*pos.x = cos(p.x) * r * (r2 + cos(p.y));
    pos.y = sin(p.x) * r * (r2 + cos(p.y));
    pos.z = sin(p.y) * r;*/

   /* pos.x = cos(p.x * 2) * r * (r2 + cos(p.x * 3));
    pos.y = sin(p.x * 2) * r * (r2 + cos(p.x * 3));
    pos.z = sin(p.x * 3) * r;*/

    float3 p1 = calculatePositionOnCurve(p.y, 2, 3, r2);
    float3 p2 = calculatePositionOnCurve(p.y + 0.01, 2, 3, r2);

    float3 t = p2 - p1;
    float3 norm = p2 + p1;
    float3 b = cross(t, norm);
    norm = cross(b, t);

    b = normalize(b);
    norm = normalize(norm);

    float cx = tube * cos(p.x);
    float cy = tube * sin(p.x);

    pos.x = p1.x + (cx * norm.x + cy * b.x);
    pos.y = p1.y + (cx * norm.y + cy * b.y);
    pos.z = p1.z + (cx * norm.z + cy * b.z);
    
    //pos.xyz *= 0.4;
    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = uvOut;
    return output;
}
