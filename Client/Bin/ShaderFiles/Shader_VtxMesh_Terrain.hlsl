//======================================================
// Shader_VtxMesh_Terrain.hlsl
// - 지형(Terrain)용: Sand 텍스처 + 기존 조명 모델 그대로 사용
// - VtxMesh 와 동일한 형식 (행렬/라이트/샘플러/구조체 등)
//======================================================

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

//  여기에 Terrain용 Sand 텍스처를 바인딩할 거야
Texture2D g_TerrainDiffuse : register(t0);

// 재질 파라미터 (Ambient / Specular)
vector g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

// 카메라 위치
vector g_vCamPosition;

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

sampler PointSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = wrap;
    AddressV = wrap;
};

//-----------------------------------------
// 정점 입력 / 출력 구조체 (VtxMesh와 동일)
//-----------------------------------------
struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

//-----------------------------------------
// Vertex Shader
//-----------------------------------------
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    // 월드 → 뷰 → 프로젝션
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    
    Out.vPosition = vPosition;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

//-----------------------------------------
// Pixel Shader
//-----------------------------------------
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    //  여기서 Sand 텍스처를 사용
    vector vMtrlDiffuse = g_TerrainDiffuse.Sample(DefaultSampler, In.vTexcoord);
   
    // 기본 디퓨즈 + 앰비언트 + 스페큘러 조명 계산 (VtxMesh와 거의 동일)
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
    
    float4 vLook = In.vWorldPos - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    
    float fSpecular = pow(max(dot(normalize(vLook) * -0.1f, vReflect), 0.f), 50);
    
    Out.vColor =
        g_vLightDiffuse * vMtrlDiffuse *
        saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
        (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

//-----------------------------------------
// Technique
//-----------------------------------------
technique11 DefaultTechnique
{
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
