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

float3 cubeToSphere(float3 p)
{
    return normalize(p);
}
float3 calcGeom(float2 uv, int faceID)
{
    float2 p = uv * 2.0 - 1.0;
    float3 cubePos;
    if (faceID == 0)      cubePos = float3(-1, p.y, p.x);
    else if (faceID == 1) cubePos = float3(1, p.y, -p.x);
    else if (faceID == 2) cubePos = float3(-p.x, -1, -p.y);
    else if (faceID == 3) cubePos = float3(-p.x, 1, p.y);
    else if (faceID == 4) cubePos = float3(-p.x, p.y, -1);
    else if (faceID == 5) cubePos = float3(p.x, p.y, 1);
    else                  cubePos = float3(0, 0, 0); // fallback
    cubePos = normalize(cubePos);
    return cubePos;
    cubePos = rotX(cubeToSphere(cubePos), time.x * 0.05);
    return rotY(cubeToSphere(cubePos), time.x * 0.05);
}

void computeSphereFrame(float2 uv, int faceID, out float3 tangent, out float3 binormal, out float3 normal)
{
    float2 step = 1.0 / float2(gx, gy);

    float3 p = calcGeom(uv, faceID);
    float3 px = calcGeom(uv + float2(step.x, 0), faceID);
    float3 py = calcGeom(uv + float2(0, step.y), faceID);

    normal = normalize(p);
    float3 up = abs(normal.y) > 0.99 ? float3(1, 0, 0) : float3(0, 1, 0);

    tangent = normalize(cross(up, normal));
    binormal = normalize(cross(normal, tangent));


}


VS_OUTPUT VS(uint vID : SV_VertexID, uint iID : SV_InstanceID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    float2 quad[6] = {
        float2(-1, -1), float2(1, -1), float2(-1, 1),
        float2(1, -1), float2(1, 1), float2(-1, 1)
    };

    float2 p = quad[vID % 6];
    int qID = vID / 6;
    int vg = (int)(gx * gy);
    int localID = qID % vg;
    int faceID = qID / vg;

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
    computeSphereFrame(uv, faceID, tangent, binormal, normal);

    binormal = normalize(binormal);
    tangent = normalize(tangent);
    normal = normalize(normal);
    int t = iID % 5 + 1;
    int s = (iID - t + 1) % 3 + 1;
    pos.x = pos.x + 9;
    pos.y = pos.y + 5;
    pos.x = pos.x - t * 3;
    pos.y = pos.y - s * 2.5;
    pos *= 0.35f;
    float3 albedo;
    float metallic;
    float roughness;
    if (t == 1.0 && s == 1.0) {
        // Хром
        albedo = float3(0.95, 0.95, 0.95);
        metallic = 1.0;
        roughness = 0.1;
    }
    else if (t == 3.0 && s == 1.0) {
        // Золото
        albedo = float3(1.00, 0.71, 0.29);
        metallic = 1.0;
        roughness = 0.3;
    }
    else if (t == 2.0 && s == 1.0) {
        // Железо
        albedo = float3(0.56, 0.57, 0.58);
        metallic = 1.0;
        roughness = 0.2;
    }
    else if (t == 4.0 && s == 1.0) {
        // Пластик (красный)
        albedo = float3(0.8, 0.1, 0.1);
        metallic = 0.0;
        roughness = 0.4;
    }
    else if (t == 5.0 && s == 1.0) {
        // Резина
        albedo = float3(0.015, 0.015, 0.015);
        metallic = 0.0;
        roughness = 0.9;
    }
    else if (s == 2.0) {
        // Для второго ряда - градация roughness
        albedo = float3(0.8, 0.8, 0.8);
        metallic = 0.5;
        roughness = t / 5.0; // От 0.2 до 1.0
    }
    else if (s == 3.0) {
        // Для третьего ряда - градация metallic
        albedo = float3(0.8, 0.8, 0.8);
        metallic = t % 2; // От 1.0 до 0.2
        roughness = 0.5;
    }
    else {
        // По умолчанию
        albedo = float3(0.8, 0.8, 0.8);
        metallic = 0.5;
        roughness = 0.5;
    }

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
