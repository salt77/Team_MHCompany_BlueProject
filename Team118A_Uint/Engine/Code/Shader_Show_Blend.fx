

/*			Albedo 텍스쳐			*/
texture		g_AlbedoTexture;
sampler		AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

/*			Emission 텍스쳐			*/
texture		g_EmissionTexture;
sampler		EmissionSampler = sampler_state
{
	texture = g_EmissionTexture;
};

/*			Shade 텍스쳐				*/
texture		g_ShadeTexture;
sampler		ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;
	AddressU = clamp;
	AddressV = clamp;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

/*			Specular 텍스쳐			*/
texture		g_SpecularTexture;
sampler		SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
};

// 픽셀 인 아웃
struct PS_IN
{
	float2		vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

// 최종
PS_OUT		PS_Result(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vAlbedo		= tex2D(AlbedoSampler, In.vTexUV);
	vector vEmission	= tex2D(EmissionSampler, In.vTexUV);
	vector vShade		= tex2D(ShadeSampler, In.vTexUV);
	//vector vSpec		= tex2D(SpecularSampler, In.vTexUV);
	
	//vAlbedo.rgb *= vShade.r;
	//vAlbedo.a = 1.f;

	// 최소값
	//float fColor = 0.4f;
	//vector vColor = fColor + vShade * (1.f - fColor);

	//if (0.2f > vShade.r)
	//{
	//	vColor -= 0.6;
	//}

	// vAlbedo *= vColor;

	Out.vColor = vAlbedo * vShade + vEmission;
		   	 
	   
	return Out;
}

technique Default_Device
{
	pass ShowResult
	{
		ZWriteEnable = false;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		
		AlphaTestEnable = false;

		//AlphaBlendEnable = false;
		//
		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 1;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_Result();
	}
};