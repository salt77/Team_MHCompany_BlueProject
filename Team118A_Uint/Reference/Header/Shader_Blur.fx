

texture		g_SampleTexture;
sampler		SampleSampler = sampler_state
{
	texture = g_SampleTexture;
	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

float g_TexW = 1280.f;
float g_TexH = 720.f;

float g_Range = 2.f; // 1.f ~ 5.f
float g_Intense = 1.1f; // 1.f ~ 1.1f

static const float Weight[13] =
{
	0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

//////////////////////////////////////////////////////

// ÇÈ¼¿ ÀÎ ¾Æ¿ô
struct PS_IN
{
	float2		vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor		:	COLOR0;
};


PS_OUT		PS_BlurX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	for (int i = -6; i < 6; ++i)
	{
		float2 uv = In.vTexUV + float2( i / g_TexW * g_Range, 0);

		Out.vColor += Weight[6 + i] * g_Intense* tex2D(SampleSampler, uv);
	}

	Out.vColor /= Total;
	
	return Out;
}


PS_OUT		PS_BlurY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	for (int i = -6; i < 6; ++i)
	{
		float2 uv = In.vTexUV + float2(0, i / g_TexH * g_Range);

		Out.vColor += Weight[6 + i] * g_Intense* tex2D(SampleSampler, uv);
	}

	Out.vColor /= Total;

	return Out;
}


technique Default_Device
{
	pass BlurX
	{
		ZEnable = false;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;
		
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BlurX();
	}

	pass BlurY
	{
		ZEnable = false;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;
		
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_BlurY();
	}
};