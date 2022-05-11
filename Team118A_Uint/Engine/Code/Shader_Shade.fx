

// 노말 텍스쳐 
texture g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

// 뎁스 텍스쳐 
texture	g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

// 그림자 텍스쳐 
texture	g_ShadowTexture;
sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

/////////////////////////////////////////////////////////////////

// 빛 정보
vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

matrix		g_matLightView;

// 카메라
vector		g_vCamPos;

// 변환 행렬
matrix		g_matProj;
matrix		g_matInvProj;
matrix		g_matView;
matrix		g_matInvView;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// SSAO 인자
bool		g_bSSAO_OnOff;
//  1

float g_fRadius = 0.05f;

float3 g_vRandom[16] =
{
	float3(0.2024537f, 0.841204f, -0.9060141f),
	float3(-0.2200423f, 0.6282339f, -0.8275437f),
	float3(0.3677659f, 0.1086345f, -0.4466777f),
	float3(0.8775856f, 0.4617546f, -0.6427765f),
	float3(0.7867433f, -0.141479f, -0.1567597f),
	float3(0.4839356f, -0.8253108f, -0.1563844f),
	float3(0.4401554f, -0.4228428f, -0.3300118f),
	float3(0.0019193f, -0.8048455f, 0.0726584f),
	float3(-0.7578573f, -0.5583301f, 0.2347527f),
	float3(-0.4540417f, -0.252365f, 0.0694318f),
	float3(-0.0483353f, -0.2527294f, 0.5924745f),
	float3(-0.4192392f, 0.2084218f, -0.3672943f),
	float3(-0.8433938f, 0.1451271f, 0.2202872f),
	float3(-0.4037157f, -0.8263387f, 0.4698132f),
	float3(-0.6657394f, 0.6298575f, 0.6342437f),
	float3(-0.0001783f, 0.2834622f, 0.8343929f),
};

float3 RandomNormal(float2 tex)
{
	float noiseX = (frac(sin(dot(tex, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
	float noiseY = (frac(sin(dot(tex, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
	float noiseZ = (frac(sin(dot(tex, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));

	return normalize(float3(noiseX, noiseY, noiseZ));
};

float3 avKernel[8] =
{
	float3(0.125f, 0.125f, 0.125f) ,
	float3(-0.250f,-0.250f,-0.250f) ,
	float3(-0.375f,-0.375f, 0.375f) ,
	float3(-0.500f, 0.500f,-0.500f) ,
	float3(-0.625f, 0.625f ,0.625f) ,
	float3(0.750f,-0.750f,-0.750f) ,
	float3(0.875f,-0.875f, 0.875f) ,
	float3(1.000f, 1.000f,-1.000f)
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

// 픽셀 인 아웃
struct PS_IN
{
	float2			vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector			vShade		: COLOR0;
	vector			vSpecular	: COLOR1;
};

PS_OUT		PS_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 쉐이드
	
	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float fDepthBlue = vDepth.b;
	// 0.5f == fDepthBlue
	float  fViewZ = vDepth.y * 1000.f;


	vector vNormal = vector(tex2D(NormalSampler, In.vTexUV) * 2.f - 1.f);
	
	float fShade = saturate(	-dot(	g_vLightDir.xyz, vNormal.xyz).r );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 툰 쉐이드
		
	float fBegin = 1.f;
	float fEnd = 0.f;
	float fMin = 0.f;
	float fDis = 0.1f;
	
	if (0.25f <= fShade || 0.5f == fDepthBlue)
	{
		fBegin = 1.f;
		fEnd = 0.25f;
		fMin = 0.9f;
	}
	else
	{
		fBegin = 0.25f;
		fEnd = 0.f;
		fMin = 0.5f;
	}
	
	Out.vShade.rgba = fMin + fDis * (fShade - fEnd) / (fBegin - fEnd);


	//// 부드럽게 각진 툰 쉐이딩
	//if (0.25f <= fShade) // 
	//{
	//	Out.vShade.rgba = 0.8f + fShade * 0.2f;
	//}
	//else if (0.2f <= Out.vShade.x)
	//{
	//	Out.vShade.rgba = 0.4f + fShade * 0.5f;
	//}
	//else // 0.2 이하
	//{
	//	Out.vShade.rgba = 0.2f + fShade * 0.3f;
	//}
	//
	//if (0.5f == fDepthBlue) 
	//{
	//	Out.vShade.rgba = 1.f;
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 스펙큘러

	vector	vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));
	vector	vWorldPos;
	
	// 텍스쳐 0 ~ 1 -> 투영 -1 ~ 1
	vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	// 텍스쳐 0 ~ 1 -> 투영 1 ~ -1
	vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	// 투영 0 ~ 1
	vWorldPos.z = vDepth.x * fViewZ;
	vWorldPos.w = fViewZ;
	
	vWorldPos = mul(vWorldPos, g_matInvProj);
	vWorldPos = mul(vWorldPos, g_matInvView);
	
	vector	vLook = normalize(vWorldPos - g_vCamPos);
	
	Out.vSpecular = pow(saturate(dot(vReflect, vLook * -1.f)), 30.f) * 0.5f * g_vLightSpecular;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 스펙큘러 보정
	
	//if (0.3f <= Out.vSpecular.x) 
	//{
	//	Out.vSpecular = Out.vSpecular * 0.5f;
	//	Out.vSpecular.rgb = float3(0.5f, 0.5f, 0.5f);
	//}

	//Out.vSpecular.a = 1.f;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 그림자
	
	vWorldPos	= mul(vWorldPos, g_matLightView);
	
	vector		vUVPos = mul(vWorldPos, g_matProj);
	float2		vNewUV;
	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;
	
	
	if (vWorldPos.z > tex2D(ShadowSampler, vNewUV).r * 1000.0f && 
		0.f == fDepthBlue)
	{
		Out.vShade.rgb *= 0.f;
		Out.vShade.a *= 0.f;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// SSAO

	// 주석
	// g_bSSAO_OnOff : CRenderer의 매개변수고 KEY_P를 Key_Down을 통해서 SSAO를 온오프가능하다.
	// CManagement의 Update에서 OnOff 를 체크하고있으니 

	float fAO = 1.f;

	if (g_bSSAO_OnOff)
	{
		int iSize = 16;

		int iColor = 0;
		for (int i = 0; i < iSize; ++i)
		{
			half3 vRay = reflect(RandomNormal(In.vTexUV), g_vRandom[i]);
			
			half3 vReflect = normalize(reflect(vRay, vNormal.xyz)) * g_fRadius;
			vReflect.x *= -1.f;

			float fOccNorm = tex2D(DepthSampler, In.vTexUV + vReflect.xy).g * fViewZ * 1000.f;

			if (fOccNorm <= vDepth.g * fViewZ  * 1000.f + 0.0003f)
			{
				++iColor;
			}
		}

		fAO =  1.f -  (		(float)iColor / (float)iSize	);
	}

	// 쉐이드 색상 결정
	Out.vShade = Out.vShade * g_vLightDiffuse + g_vLightAmbient * fAO;	
	Out.vShade.a = 1.f;
	
	return Out;
}

PS_OUT PS_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// 점 조명 아직 코드 없음 
	
	return Out;
}


technique Default_Device
{
	pass Directional
	{
		ZWriteEnable = false;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_DIRECTIONAL();
	}
	pass Point
	{
		ZWriteEnable = false;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_POINT();
	}
};