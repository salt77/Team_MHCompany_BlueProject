

// 기본 행렬
matrix	g_matWorld, g_matView, g_matProj;

float	g_fFar = 1000.f;
bool	g_bBlackImage = false;

// Albedo 텍스쳐
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 디퓨즈 랜더 쉐이더

// 버텍스 인 아웃
struct VS_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector		vColor		: COLOR0;
	vector		vRight		: COLOR1;
	vector		vUp			: COLOR2;
	vector      vLook		: COLOR3;
	vector		vPos		: COLOR4;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
	vector		vWorldPos	: TEXCOORD2;
	vector		vColor		: COLOR0;
};

// 버텍스 쉐이더
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWorld;
	
	matWorld._11 = In.vRight.x;
	matWorld._12 = In.vRight.y;
	matWorld._13 = In.vRight.z;
	matWorld._14 = In.vRight.w;
				 
	matWorld._21 = In.vUp.x;
	matWorld._22 = In.vUp.y;
	matWorld._23 = In.vUp.z;
	matWorld._24 = In.vUp.w;
				 
	matWorld._31 = In.vLook.x;
	matWorld._32 = In.vLook.y;
	matWorld._33 = In.vLook.z;
	matWorld._34 = In.vLook.w;
				  
	matWorld._41 = In.vPos.x;
	matWorld._42 = In.vPos.y;
	matWorld._43 = In.vPos.z;
	matWorld._44 = In.vPos.w;

	matrix matWV, matWVP;
	matWV = mul(matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	Out.vTexUV = In.vTexUV;
	Out.vColor = In.vColor;
	return Out;
}

// 픽셀 아웃
struct PS_OUT
{
	vector		vColor		:	COLOR0;
	vector		vEmission	:	COLOR1;
	vector		vNormal		:	COLOR2;
	vector		vDepth		:	COLOR3;
};

// 픽셀 쉐이더
PS_OUT		PS_MAIN_Light(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = vColor;

	if (g_bBlackImage)
	{
		Out.vColor.a = Out.vColor.r;
	}
	Out.vColor *= In.vColor;
	
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}



// 패스
technique Default_Device
{
	pass Light
	{
		/*	
		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		*/

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0x000000c0;

		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_Light();
	}
};