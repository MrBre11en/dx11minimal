cbuffer global : register(b5)
{
    float4 gConst[32];
};

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
    float2 iResolution;
    float2 pad;
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

cbuffer objParams : register(b0)
{
    float gx;
    float gy;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 normal : NORMAL1;
    float4 tangent : NORMAL2;
    float4 binormal : NORMAL3;
    float2 uv : TEXCOORD0;
    float2 metallic : TEXCOORD1;
    float4 albedo : TEXCOORD2;
    float2 roughness : TEXCOORD3;
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

#define PI 3.1415926535897932384626433832795

float length(float3 c)
{
    float x = c.x;
    float y = c.y;
    float z = c.z;
    float l = sqrt(x * x + y * y + z * z);
    return l;
}


VS_OUTPUT VS(VS_INPUT v_info, uint iID : SV_InstanceID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    float2 p = v_info.pos;

    int px = localID % (int)gx;
    int py = localID / (int)gx;

    float2 uv = float2(px + 0.5 + p.x * 0.5, py + 0.5 + p.y * 0.5) / float2(gx, gy);
    float2 step = 1 / float2(gx, gy);
    float2 uv1 = uv + float2(step.x, 0);
    float2 uv2 = uv + float2(0, step.y);


    float3 pos = calcGeom(uv, faceID);
    float3 pos1 = calcGeom(uv1, faceID);
    float3 pos2 = calcGeom(uv2, faceID);
    float3 tangent, binormal, normal;

    int t = iID % 5 + 1;
    int s = (iID - t + 1) % 3 + 1;
    pos.x = pos.x + 9;
    pos.y = pos.y + 5;
    pos.x = pos.x - t * 3;
    pos.y = pos.y - s * 2.5;
    pos *= 0.35f;

    float3 albedo = float3(0.8, 0.8, 0.8);
    float metallic = 0.5;
    float roughness = 0.5;

    output.wpos = float4(pos, 1.0);
    output.vpos = mul(float4(pos, 1.0), view[0]);
    output.pos = mul(float4(pos, 1.0), mul(view[0], proj[0]));
    output.normal = float4(normal, 1.0);
    output.tangent = float4(tangent, 1.0);
    output.binormal = float4(binormal, 1.0);
    output.uv = uv;
    output.metallic = float2(metallic, 1);
    output.albedo = float4(albedo, 1);
    output.roughness = float2(roughness, 1);
    return output;
}
