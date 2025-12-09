#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D g_DiffuseTexture;

vector g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

int g_bUseCustomColor = 0; // 0: 텍스처 색, 1: 커스텀 색
vector g_vCustomColor = vector(1.f, 1.f, 1.f, 1.f); // 사용할 색

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
    float3 vLocalPos : TEXCOORD2;
};

/* 정점의 기본적인 변환을 가져간다. World, View, Proj */ 
/* 정점의 구성 정보를 변형한다. (멤버를 늘리거나 , 줄이거나 ) */ 

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;   
    
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);  
    
    Out.vPosition = vPosition;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    Out.vLocalPos = In.vPosition;
    
    return Out;        
}

/* 포지션시멘틱에 해당하는 변수의 w로 모든 성분을 나눈다 */
/* 뷰포트로 변환한다.(윈도우좌표로 변환) */
/* 래스터라이즈 : 리턴된 정점정보를 기반으로하여 사이를 선형보간한 픽셀을 만들어준다.  */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float3 vLocalPos : TEXCOORD2;

};

struct PS_OUT
{
    vector vColor : SV_TARGET0;    
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;       
    
    vector vMtrlDiffuse;// = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    bool isTrunk = false;
    
    if (g_bUseCustomColor == 1) //트리만ㅋㅋ색상 변경
    {
        float height = In.vLocalPos.y; // 로컬 기준 높이
        float radial = length(In.vLocalPos.xz); // 중심에서의 반지름

        // 값은 대충 잡고, 나중에 보고 조절하면 돼
        float trunkTopHeight = 30.0f; // 줄기가 끝나는 높이
        float trunkRadius = 2.f; // 줄기 반지름 (야자수 몸통 두께 느낌)

        isTrunk = (height < trunkTopHeight) && (radial < trunkRadius);

        vector trunkColor = vector(0.45f, 0.28f, 0.15f, 1.f); // 갈색
        vector leafColor = vector(0.15f, 0.6f, 0.2f, 1.f); // 초록

        if (isTrunk)
            vMtrlDiffuse = trunkColor;
        else
            vMtrlDiffuse = leafColor;

    }
    else
    {
        vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    }
    
    float3 N = normalize(In.vNormal.xyz);

    if (g_bUseCustomColor == 1 && isTrunk)
    {
        float3 noisePos = In.vWorldPos.xyz * 0.1f; // ★ 공간을 10배 키움 → 노이즈 패턴도 10배 커짐
       // float noise = frac(sin(dot(noisePos, float3(12.9898f, 78.233f, 37.719f))) * 43758.5453f);
       // noise = (noise - 0.5f) * 0.3f; // 흔들림 강도도 조금 낮춤
       //
       // // 노멀에 살짝 더해준다
       // N = normalize(N + float3(noise, noise, noise));
    }
    float3 L = normalize(-g_vLightDir.xyz); // 광원 방향
    float3 V = normalize(g_vCamPosition.xyz - In.vWorldPos.xyz); // 눈 방향
    float3 R = reflect(-L, N);

    float fShade = max(dot(L, N), 0.0f);

    // 스페큘러 파워/세기 조절해서 덜 번쩍거리게
    float fSpecPower = 16.0f; // 원래 50 → 16 (하이라이트 넓게)
    float fSpecStrength = 0.3f; // 스페큘러 강도 줄이기

    float fSpecular = pow(max(dot(V, R), 0.0f), fSpecPower) * fSpecStrength;

    Out.vColor =
        g_vLightDiffuse * vMtrlDiffuse *
        saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
        (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    { 
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        VertexShader = compile vs_5_0 VS_MAIN();       
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}