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

cbuffer params : register(b1)
{
    float r, g, b;
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

/////////////////////////////////////////////////////////

//random noise function
float nrand(float2 n)
{
    return frac(sin(dot(n.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float sincosbundle(float val)
{
    return sin(cos(2. * val) + sin(4. * val) - cos(5. * val) + sin(3. * val)) * 0.05;
}


//color function
float3 getcolor(float2 uv)
{
    float zoom = 10.;
    float3 brickColor = float3(0.45, 0.29, 0.23);
    float3 lineColor = float3(0.845, 0.845, 0.845);
    float edgePos = 1.5;

    //grid and coord inside each cell
    float2 coord = floor(uv);
    float2 gv = frac(uv);

    //for randomness in brick pattern, it could be better to improve the color randomness
    //you can try to make your own
    float movingValue = -sincosbundle(coord.y) * 2.;

    //for the offset you can also make it more fuzzy by changing both
    //the modulo value and the edge pos value
    float offset = floor(fmod(uv.y, 2.0)) * (edgePos);
    float verticalEdge = abs(cos(uv.x + offset));

    //color of the bricks
    float3 brick = brickColor - movingValue;


    bool vrtEdge = step(1. - 0.01, verticalEdge) == 1.;
    bool hrtEdge = gv.y > (0.9) || gv.y < (0.1);

    if (hrtEdge || vrtEdge)
        return lineColor;
    return brick;
}

//normal functions
//both function are modified version of https://www.shadertoy.com/view/XtV3z3
float lum(float2 uv) {
    float3 rgb = getcolor(uv);
    return 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b;
}

float3 normal(float2 uv, float3x3 tbn) {

    float zoom = 10.;
    float3 brickColor = float3(0.45, 0.29, 0.23);
    float3 lineColor = float3(0.845, 0.845, 0.845);
    float edgePos = 1.5;

    //edge normal, it mean it's the difference between a brick and the white edge
    //higher value mean bigger edge
    float r = 0.03;

    float x0 = lum(float2(uv.x + r, uv.y));
    float x1 = lum(float2(uv.x - r, uv.y));
    float y0 = lum(float2(uv.x, uv.y - r));
    float y1 = lum(float2(uv.x, uv.y + r));

    //NOTE: Controls the "smoothness"
    //it also mean how hard the edge normal will be
    //higher value mean smoother normal, lower mean sharper transition
    //tbn = mul(tbn, transpose(view[0]));

    float s = 1.0;
    float3 nn = normalize(float3(x1 - x0, y1 - y0, s));
    nn.y *= -1;
    float3 n = normalize(mul(nn, tbn));

    //return input.vnorm.xyz;

    return (n);

}

/// ////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433832795

float3 env(float3 v)
{
    //float aXZ = atan2(v.y, v.z);
    //float aXY = atan2(v.y, v.x);
    //float t = sin(aXZ * 44) + sin(aXY * 44);
     //   return float3(t, t, t);

    float a = .9 * saturate(1244 * sin((v.z / v.y) * 6) * sin((v.x / v.y) * 6));
    float blend = saturate(8 - pow(length(v.xz / v.y), .7));

    float va = atan2(v.z, v.x);
    float x = frac(va / PI / 2 * 64);
    float y = frac((v.y) * 10. + .5);

    float b = saturate(1 - 2 * length(float2(x, y) - .5));

    a = lerp(a, b, 1 - blend);
    a = pow(a, 24) * 10;
    a *= saturate(-v.y);
    a += saturate(1 - 2 * length(v.xz)) * saturate(v.y) * 44;
    return float3(a, a, a);
}

/// ////////////////////////////////////////////////////


float4 PS(VS_OUTPUT input) : SV_Target
{
//float2 brick_uv = float2(30, 200);
    //float3 light = float3(0.5, -0.5, -0.5);
   // float3 light = normalize(float3(1, -1, -0.7));
   // float specular_strength = 2;

    //float3 fn = normal(input.uv * brick_uv, tbn);
    //fn *= float3(1, -1, 1);

    float3x3 tbn =
    {
        input.tangent,
        input.binormal,
        float3(input.vnorm.xyz)
    };

    float3 vnorm = float3(input.vnorm.xyz);
    //vnorm *= float3(1, -1, 1);
    //float3 camera_pos = float3(0, 0, -1);

    //float3 ambient = float3(0.1, 0.1, 0.1);

    float3 viewDir = input.wpos.xyz - camPos;
    //float3 viewDir = mul(input.wpos, view[0]);

    float3 reflectDir = normalize(reflect(-viewDir, vnorm));

/*    float3 diffuse = saturate(dot(light, vnorm));
    float3 color = float3(1, 1, 1);

    float spec = pow(max(dot(input.vpos.xyz, reflectDir), 0), 32);
    float3 specular = specular_strength * spec * color;

    float pi = 3.141519;
    float2 uv = input.uv;

    float4 lighting = float4(ambient + diffuse + specular, 1);
    */
    float3 rc = env(reflectDir);

    //return (input.vnorm/2+.5);
    return float4(rc, 1);

    //return lighting;
    //return float4(frac(input.uv.x+time.x*.01), 0, 0, 1);

    //uv *= zoom;
    //uv.y += iTime;

    //get normal with normal(uv)
    //get color with color(uv)


    //return float4((ambient + diffuse + specular)*getcolor(uv * brick_uv), 1.0);

}