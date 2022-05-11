

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


// 빛 정보
vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

matrix		g_matLightView;

// 카메라
vector		g_vCamPos;

// 변환 행렬
matrix		g_matInvProj;
matrix		g_matInvView;


// 픽셀 인 아웃
struct PS_IN
{
	float2			vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	vector			vShade		: COLOR0;
	vector			vSpec		: COLOR1;
};

PS_OUT		PS_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);
	float	fDepthBlue = vDepth.b;
	float	fViewZ = vDepth.y * 1000.f;

	//float3 vDir = g_vLightDir.xyz;
	//float3 vDiffuse = g_vLightDiffuse.rgb;
	//float3 vAmbient = g_vLightAmbient.rgb;

	float3 vDir		= float3(0.2f,	-1.f,	0.2f);
	float3 vDiffuse = float3(1.f,	0.96f,	0.93f);
	float4 vSpec	= g_vLightSpecular;
	float3 vAmbient = float3(0.1f,	0.1f,	0.1f);
	vector vNormal	= vector(tex2D(NormalSampler, In.vTexUV) * 2.f - 1.f);	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	float fShade = saturate(	-dot(vDir, vNormal.xyz).r );

	float fBegin = 1.f;
	float fEnd = 0.f;
	float fMin = 0.f;
	float fMax = 1.f;

	if (0.25f <= fShade || 0.5f == fDepthBlue)
	{
		fBegin = 1.f;
		fEnd = 0.25f;
		fMin = 0.95f;
		fMax = 1.f;
	}
	else
	{
		fBegin = 0.25f;
		fEnd = 0.f;
		fMin = 0.5f;
		fMax = 0.6f;
	}

	Out.vShade = fMin + (fMax - fMin) * (fShade - fEnd) / (fBegin - fEnd);

	Out.vShade.rgb = Out.vShade.rgb * vDiffuse + vAmbient;
	Out.vShade.a = 1.f;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//vector	vReflect = normalize(reflect(normalize(vector(vDir, 0.f)), vNormal));
	//vector	vWorldPos;
	//
	//vWorldPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	//vWorldPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	//vWorldPos.z = vDepth.x * fViewZ;
	//vWorldPos.w = fViewZ;
	//
	//vWorldPos = mul(vWorldPos, g_matInvProj);
	//vWorldPos = mul(vWorldPos, g_matInvView);
	//
	//vector	vLook = normalize(vWorldPos - g_vCamPos);

	//Out.vSpec = vector(1.f, 0.f, 0.f, 1.f);
	//Out.vSpec = pow(saturate(dot(vReflect, vLook * -1.f)), 30.f) * 0.5f * g_vLightSpecular;
	//Out.vSpec.a = 0.2f;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return Out;
}


technique Default_Device
{
	pass ShowDirectional
	{
		ZWriteEnable = false;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_DIRECTIONAL();
	}
};