//cbuffer InstanceData : register(b6)
//{
//    int index;
//};
//
//cbuffer global : register(b5)
//{
//    float4 gConst[32];
//};
//
//cbuffer frame : register(b4)
//{
//    float4 time;
//    float4 aspect;
//};
//
//cbuffer camera : register(b3)
//{
//    float4x4 world[2];
//    float4x4 view[2];
//    float4x4 proj[2];
//};
//
//cbuffer drawMat : register(b2)
//{
//    float4x4 model;
//    float hilight;
//};
//
//cbuffer params : register(b1)
//{
//    float r, g, b;
//};
//
//#define PI 3.1415926535897932384626433832795
//
//struct VS_OUTPUT
//{
//    float4 pos : SV_POSITION;
//    float4 vpos : POSITION0;
//    float4 wpos : POSITION1;
//    float4 normal : NORMAL1;
//    float4 tangent : NORMAL2;
//    float4 binormal : NORMAL3;
//    float2 uv : TEXCOORD0;
//    float2 metallic : TEXCOORD1;
//    float4 albedo : TEXCOORD2;
//    float2 roughness : TEXCOORD3;
//};
//
//float3 FresnelSchlick(float cosTheta, float3 F0)
//{
//    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
//}
//
//float DistributionGGX(float3 N, float3 H, float roughness)
//{
//    float a = roughness * roughness;
//    float a2 = a * a;
//    float NdotH = max(dot(N, H), 0.0);
//    float NdotH2 = NdotH * NdotH;
//
//    float nom = a2;
//    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
//    denom = PI * denom * denom;
//
//    return nom / denom;
//}
//
//float GeometrySchlickGGX(float NdotV, float roughness)
//{
//    float r = (roughness + 1.0);
//    float k = (r * r) / 8.0;
//
//    float nom = NdotV;
//    float denom = NdotV * (1.0 - k) + k;
//
//    return nom / denom;
//}
//
//float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
//{
//    float NdotV = max(dot(N, V), 0.0);
//    float NdotL = max(dot(N, L), 0.0);
//    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
//    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
//
//    return ggx1 * ggx2;
//}
//
//float4 PS(VS_OUTPUT input) : SV_Target
//{
//
//    //float st = input.singlePos.x;
//
//    float3 N = normalize(input.normal.xyz);
//
//    float3 fragPos = input.wpos.xyz;
//    float3 cameraPos = -float3(view[0]._m02, view[0]._m12, view[0]._m22)* view[0]._m32;
//    float3 V = normalize(fragPos- cameraPos);
//    float3 L = normalize(float3(0, 1, 0)); 
//    float3 H = normalize(V + L); 
//    float3 T = normalize(input.tangent.xyz);
//    float3 B = normalize(input.binormal.xyz);
//
//    float3 albedo = input.albedo.xyz;
//    float metallic = input.metallic.x;
//    float roughness = input.roughness.x;
//  //   if (st == 2.0 ) {
  //      // Железо
  //      albedo = float3(0.56, 0.57, 0.58);
  //      metallic = 1.0;
  //      roughness = 0.2;
  //  }
  //   else {
  //       // По умолчанию
  //       albedo = float3(0.8, 0.8, 0.8);
  //       metallic = 0.5;
  //       roughness = 0.5;
  //   }
  //  else if (s == 3.0 && s == 1.0) {
  //      // Пластик (красный)
  //      albedo = float3(0.8, 0.1, 0.1);
  //      metallic = 0.0;
  //      roughness = 0.4;
  //  }
  //  else if (s == 4.0 && s == 1.0) {
  //      // Резина
  //      albedo = float3(0.05, 0.05, 0.05);
  //      metallic = 0.0;
  //      roughness = 0.9;
  //  }
  //  else if (s == 5.0 && s == 1.0) {
  //      // Хром
  //      albedo = float3(0.95, 0.95, 0.95);
  //      metallic = 1.0;
  //      roughness = 0.1;
  //  }
  //  else {
  //      // По умолчанию
  //      albedo = float3(0.8, 0.8, 0.8);
  //      metallic = 0.5;
  //      roughness = 0.5;
  //  }
    // albedo = float3(0.56, 0.57, 0.58);
    // metallic = 1.0;
    // roughness = 0.2;
//    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
//
//    float cosTheta = max(dot(-V, N), 0.0);
//    float3 fresnel = FresnelSchlick(cosTheta, F0);
//    
//    // return float4(1 - fresnel, 1);
//
//   // float3 N_color = float3(normalize(st * 0.5 + 0.5),0);
//    float3 kS = fresnel;           // Скол
//    float3 kD = 1.0 - kS;           // Ско
//    kD *= 1.0 - metallic;           // Мет
//
//    float3 diffuse = albedo / PI;   // Лам
//
//    float3 finalColor = kD * diffuse + kS;
//    return float4(finalColor, 1.0);

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

struct PS_INPUT
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
    float r = 0.06;

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

float3 CosineSampleHemisphere(float2 Xi) {
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt(1.0 - Xi.y);
    float sinTheta = sqrt(Xi.y);

    return float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

float2 Hammersley(uint i, uint N) {
    uint bits = (i << 16u) | (i >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    float rdi = float(bits) * 2.3283064365386963e-10;
    return float2(float(i) / float(N), rdi);
}

float3 SampleDiffuseEnv(float3 N, float3 tangent, float3 binormal)
{
    const uint numSamples = 64;
    float3 irradiance = 0;

    for (uint i = 0; i < numSamples; i++) {
        float2 Xi = Hammersley(i, numSamples);
        float3 localSample = CosineSampleHemisphere(Xi);


        float3 worldSample = normalize(
            tangent * localSample.x +
            binormal * localSample.y +
            N * localSample.z
        );

        irradiance += env(worldSample);
    }

    return irradiance / numSamples;
}

float random(float2 st)
{
    return frac(sin(dot(st.xy,
        float2(12.9898, 78.233)))
        * 43758.5453123) * 2 - 1;
}
float random_unsigned(float2 st)
{
    return frac(sin(dot(st.xy,
        float2(12.9898, 78.233)))
        * 43758.5453123);
}

float fresnelSchlickRoughness(float cosTheta, float f0, float roughness)
{
    return f0 + (max(1 - roughness, f0) - f0) * pow(1.0 - cosTheta, 5.0);
}

// GGX importance sampling (возвращает полусферическое направление с учетом roughness)
float3 GGXSample(float2 Xi, float roughness, float3 N, float3 tangent, float3 binormal) {
    float a = roughness * roughness;

    // Сферические координаты в касательном пространстве
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // Вектор в касательном пространстве
    float3 H;
    H.x = sinTheta * cos(phi);
    H.y = sinTheta * sin(phi);
    H.z = cosTheta;

    // Переход в мировые координаты через TBN-базис
    return normalize(tangent * H.x + binormal * H.y + N * H.z);
}

float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

/// ////////////////////////////////////////////////////

float4 PS(PS_INPUT input) : SV_Target
{
    return float4(1, 1, 1, 1);

    float3 vnorm = normalize(input.normal.xyz);
    float3 tangent = normalize(input.tangent.xyz);
    float3 binormal = normalize(input.binormal.xyz);
    float3x3 tbn = float3x3(tangent, binormal, vnorm);

    float2 brick_uv = float2(10, 10);
    //float3 light = float3(0.5, -0.5, -0.5);
   // float3 light = normalize(float3(1, -1, -0.7));
   // float specular_strength = 2;

    float3 fn = normal(input.uv * brick_uv, tbn);

    float3 albedo = input.albedo.xyz * getcolor(input.uv * brick_uv);
    float metallic = input.metallic.x;
    float roughness = input.roughness.x;

    //vnorm *= float3(1, -1, 1);
    //float3 camera_pos = float3(0, 0, -1);
    //float cosTheta = dot(lightDir, N);
    //float3 ambient = float3(0.1, 0.1, 0.1);

    float3 eye = -(view[0]._m02_m12_m22) * view[0]._m32;
    float3 viewDir = normalize(eye - input.wpos.xyz);

    //float3 reflectDir = normalize(reflect(viewDir, fn));

    /*
    float3 diffuse = saturate(dot(light, fn));
    float3 color = float3(1, 1, 1);

    float spec = pow(max(dot(input.vpos.xyz, reflectDir), 0), 32);
    float3 specular = specular_strength * spec * color;

    float pi = 3.141519;
    float2 uv = input.uv;

    float4 lighting = float4(ambient + diffuse + specular, 1);
    */

    //roughness = pow(roughness, 2);
    float3 baseF0 = float3(0.04, 0.04, 0.04);
    float3 F0 = lerp(baseF0, albedo, metallic);

    // диффузное освещение
    float3 diffuseIrradiance = SampleDiffuseEnv(fn, tangent, binormal);

    // отражения
    const uint SAMPLE_COUNT = 128;
    float3 specularReflection = float3(0, 0, 0);

    for (uint i = 0u; i < SAMPLE_COUNT; i++)
    {
        float2 Xi = Hammersley(i, SAMPLE_COUNT);
        float3 H = GGXSample(Xi, roughness, fn, tangent, binormal);
        float3 L = normalize(2.0 * dot(viewDir, H) * H - viewDir);

        if (dot(fn, L) > 0.0)
        {
            float3 halfway = normalize(viewDir + L);
            float NdotL = max(dot(fn, L), 0.0);
            float VdotH = max(dot(viewDir, halfway), 0.0);

            // Вычисляем Fresnel-Schlick
            float3 fresnel = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
                    
            specularReflection += env(L) * fresnel * NdotL;
        }
    }

    specularReflection /= float(SAMPLE_COUNT);

    // Энергетическая консервация: баланс между отражённым и рассеянным светом
    float3 kS = F0;
    float3 kD = (1.0 - kS) * (1.0 - metallic);

    // Финальный цвет
    float3 diffuse = kD * albedo * (1.0 / PI) * diffuseIrradiance;
    float3 finalColor = diffuse + specularReflection;

    finalColor = ACESFilm(finalColor);
    finalColor = pow(finalColor, 1.0 / 2.2);
    return float4(finalColor, 1.0);
    //return (input.vnorm/2+.5);
}

// return float4(st,0, 1.0);

//  float3 kS = fresnel;           // Сколько отражает
//  float3 kD = 1.0 - kS;           // Сколько рассеивает
//  kD *= 1.0 - metallic;           // Металлы НЕ дают диффуза

 // float3 diffuse = albedo / PI;   // Ламбертовская модель диффуза

//  float3 finalColor = kD * diffuse + kS;

  // float ao = 1.0; // Ambient occlusion
//   // F0 - базовый коэффициент отражения при нулевом угле
//   float3 F0 = float3(0.04, 0.04, 0.04);
//   F0 = lerp(F0, albedo, metallic);
//
//   float3 radiance = float3(1.0, 1.0, 1.0); // Интенсивность света
//
//   // BRDF (Bidirectional Reflectance Distribution Function)
//   float NDF = DistributionGGX(N, H, roughness);
//   float G = GeometrySmith(N, V, L, roughness);
//   float edgeFactor = 1.5; 
//   float3 F = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, V), 0.0), 5.0) * edgeFactor;
//
//   float3 kS = F; // Коэффициент зеркального отражения
//   float3 kD = float3(1.0, 1.0, 1.0) - kS; // Коэффициент диффузного отражения
//   kD *= 1.0 - metallic;
//
//   float3 numerator = NDF * G * F;
//   float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
//   float3 specular = numerator / denominator;
//
//   // Угол между нормалью и светом
//   float NdotL = max(dot(N, L), 0.0);
//
//   // Итоговый цвет
//   float3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
//
//   // Ambient
//   float3 ambient = float3(0.09, 0.09, 0.09) * albedo * ao;
//
//   float3 color = ambient + Lo;
//
//   // Тоновая коррекция (tone mapping)
//   color = color / (color + float3(1.0, 1.0, 1.0));
//   // Гамма-коррекция
//   float gamma = 1.6;
//   color.rgb = pow(color.rgb, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));
//    return float4(color, 1.0);



   // float3 fragPos = input.wpos.xyz;
   // float3 lightDir = float3(0, 1, 0);
   // float3  lightColor = float3(23.47, 21.31, 20.79);
   // float3  Wi = normalize(lightDir - fragPos);
   // float cosTheta = max(dot(N, Wi), 0.0);
   // float attenuation = calculateAttenuation(fragPos, lightDir);
   // float3 radiance = lightColor * attenuation * cosTheta;
   // float3 L = normalize(lightDir - WorldPos);
   // float3 H = normalize(V + L);
   // float3 N_color = T * 0.5 + 0.5;
   // //return float4(N_color , 1.0);
   // float3 metalness = float3(0, 0, 0);
   // float3 F0 = float3(0.04, 0.04, 0.04);
   // float3 surfaceColor=(1,0,0)
   // F0 = mix(F0, surfaceColor.rgb, metalness);
   // float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
  //  //return float4(normalize(N.xyz), 1);
  //
  //  float3 lightDir = normalize(float3(0, 0, -1));
  //  float cosTheta = dot(lightDir, N);
  //  float F0 = (0.04, 0.04, 0.04);
  //  float3 baseColor = float3(0.5, 0.5, 0.5);
  //  //float2 brickUV = input.uv * float2(10, 10);
  // // float2 uv = input.uv;
  //
  //  //float3 texNormal = normal(brickUV) * 2.0 - 1.0;
  //
  //  //float3x3 TBN = float3x3(T, B, N);
  //  //float3 finalNormal = mul(texNormal, TBN);
  //  //finalNormal = N;
  // // float3x3 vm = (float3x3)view[0];
  //  //finalNormal = mul(finalNormal,vm);
  //
  //  float3 N_color = B * 0.5 + 0.5;
  //  //float3 B_color = B * 0.5 + 0.5;
  //  //float3 T_color = T * 0.5 + 0.5;
  //
  //  //float3 baseColor = color(brickUV);
  //
  //  float3 pos = input.wpos.xyz;
  //  float4x4 invView = saturate(view[0]);
  //  float3 cameraPos = invView._m03_m13_m23.xyz;
  //  //cameraPos.x = cameraPos+x-6;
  //  //cameraPos.y = cameraPos + y-3;
  //  float3 lightColor=(1, 1, 1);
  //  //float3 lightPos = normalize(float3(0, 1, 0));
  //  float distance = length(lightDir - pos);
  //  float attenuation = 1.0 / (distance * distance);
  //  float roughness =  1- SinglePos.y/10;
  //  float3 radiance = lightColor * attenuation;
  //
  //  float3 L = normalize(lightDir - pos);
  //  float3 V = normalize(cameraPos - pos);
  //
  //  float3 H = normalize(L + V);
  // // float3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
  //  float metallic = SinglePos%2;
  //
  //  float3 ref = reflect(V, N);
  //  float3 env = sfMap(ref);
  //  //float3 env = sfMap(N);
  //  float roug_sqr = roughness * roughness;
  //  //float3 G = CookTorrance_GGX(N, L, V ,roughness,F0, metallic);
  //  float3 G = CookTorrance_GGX(N, lightDir, V, 0, 1, 1);
  //  float3 OutColor =  G;
  // // float3 p = CookTorrance_GGX(N, L, V, roughness, F0);
  //  
  //
  //  OutColor = dot(N, lightDir);
  //
  //  
  //  //return float4(frac(input.uv * 8), 0, 1);
  //  
  //  //return float4(N_color,1);
  //
  //
  //
  ////  return float4(p,p,p, 1.0);
  //  return float4(N / 2 + .5, 1.0);
