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
    float4 camPos;
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
    float3 tangent : TANGENT1;
    float3 binormal : BINORMAL1;
    float2 uv : TEXCOORD0;
};

float3 rotY(float3 pos, float a)
{
    float3x3 m = float3x3(
        cos(a), 0, sin(a),
        0, 1, 0,
        -sin(a), 0, cos(a)
    );
    return mul(pos, m);
}

float3 rotX(float3 pos, float a)
{
    float3x3 m = float3x3(
        1, 0, 0,
        0, cos(a), -sin(a),
        0, sin(a), cos(a)
    );
    return mul(pos, m);
}

float3 rotZ(float3 pos, float a)
{
    float3x3 m = float3x3(
        cos(a), sin(a), 0,
        -sin(a), cos(a), 0,
        0, 0, 1
    );
    return mul(pos, m);
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

float3 torus_knot(float2 p)
{
    float r = 1;
    float r2 = 2;
    float tube = 0.4;
    p.x = (p.x / gx) * 3.1415926536;
    p.y = (p.y / gy) * 3.1415926536 * 2;


    //float4 pos = float4(p.x, p.y, 0, 1);
    float3 pos = float3(0, 0, 0);

    float3 p1 = calculatePositionOnCurve(-p.y, 2, 3, r2);
    float3 p2 = calculatePositionOnCurve(-p.y + 0.01, 2, 3, r2);

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

    pos = rotY(pos, time.x * 0.05);

    return pos;
}

float3 ball(float2 p)
{
    float r = 2.5;
    p.x = (p.x / gx) * 3.1415926536;
    p.y = (p.y / gy) * 3.1415926536;


    float3 pos = float3(cos(p.x) * cos(p.y) * r, sin(p.y) * r, sin(p.x) * cos(p.y) * r);


    //pos = clamp(pos, -2 ,2);

    //pos = rotZ(pos, time.x * 0.05);
    //pos = rotX(pos, time.x * 0.05);


    return pos;
}

float3 plane(float2 p)
{
    float r = 2;
    p.x = (p.x / gx);
    p.y = (p.y / gy) * 1.5;


    float3 pos = float3(p.x * r, p.y * r, p.x * r);

    pos = rotY(pos, time.x * 0.05);

    return pos;
}

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    float2 quad[6] = { -1, -1, 1, -1, -1, 1, 1, -1, 1, 1, -1, 1 };
    uint n = vID / 6;
    float2 p = quad[vID % 6];

    uint offsetX = (n % (int)gx) * 2;
    uint offsetY = (n / (int)gx) * 2;
    p.x += offsetX - (gx - 1);
    p.y += offsetY - (gy - 1);

    float2 uvOut = p.xy / float2(gx, gy) / 2 + .5;

    float3 pos = ball(p);
    float3 pos1 = ball(p + float2(1 / gx, 0));
    float3 pos2 = ball(p + float2(0, 1 / gy));

    float3 t = normalize(pos1 - pos);
    float3 b = normalize(pos2 - pos);
    float3 h = cross(t, b);
    
   
    pos.xyz *= 0.9;
    output.pos = mul(float4(pos.xyz, 1), mul(view[0], proj[0]));
    output.vpos = mul(output.pos, view[0]);
    output.wpos = float4(pos.xyz, 1);
    output.uv = uvOut;
    output.tangent = t;
    output.binormal = b;
    //output.vnorm = float4(mul(h.xyz, transpose(view[0])).xyz,1);
    output.vnorm = float4(h, 1);
    
    return output;
}
