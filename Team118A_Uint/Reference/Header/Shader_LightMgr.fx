

// 기본 행렬
matrix	g_matWorld;
// GodRay의 색상
vector	g_vColor;

// Albedo 텍스쳐
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

// 버텍스 인 아웃
struct VS_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
};

// 버텍스 쉐이더
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vTexUV = In.vTexUV;
	
	return Out;
}

// 픽셀 아웃
struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

// 픽셀 쉐이더
PS_OUT		PS_MAIN(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	// 이미지의 색상을 무조건 흰색으로 취급하겠다 -> 모서리가 까매지는 문제 떄문
	//Out.vColor = vector(1.f, 1.f, 1.f, tex2D(BaseSampler, In.vTexUV).a)  * g_vColor;
	Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vColor;
	
	return Out;
}

// 패스
technique Default_Device
{
	pass LightRay
	{
		ZEnable = TRUE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass SunRay
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};
