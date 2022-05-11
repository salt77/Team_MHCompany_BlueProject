

/*			Effect 텍스쳐			*/
texture		g_EffectTexture;
sampler		EffectSampler = sampler_state
{
	texture = g_EffectTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

/*			Effect_Blur 텍스쳐			*/
texture		g_EffectBlurTexture;
sampler		EffectBlurSampler = sampler_state
{
	texture = g_EffectBlurTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

/*			블러용					*/
//float		Weight[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
float		Weight[25] = { 0.056f, 0.085f, 0.135f, 0.205f, 0.278f, 0.390f, 0.487f, 0.605f, 0.726f, 0.825f, 0.923f, 0.96f, 1.f,
							0.96f, 0.923f, 0.825f, 0.726f, 0.605f, 0.487f, 0.390f, 0.278f, 0.205f, 0.135f, 0.085f, 0.056f };
float		WeightI[25] = { 0.056f, 0.085f, 0.135f, 0.205f, 0.278f, 0.390f, 0.487f, 0.605f, 0.726f, 0.825f, 0.923f, 0.96f, 1.f,
							0.96f, 0.923f, 0.825f, 0.726f, 0.605f, 0.487f, 0.390f, 0.278f, 0.205f, 0.135f, 0.085f, 0.056f };
float		WeightJ[25] = { 0.056f, 0.085f, 0.135f, 0.205f, 0.278f, 0.390f, 0.487f, 0.605f, 0.726f, 0.825f, 0.923f, 0.96f, 1.f,
							0.96f, 0.923f, 0.825f, 0.726f, 0.605f, 0.487f, 0.390f, 0.278f, 0.205f, 0.135f, 0.085f, 0.056f };

// CutLine
float	g_fCutLine_Begin;
float	g_fCutLine_End;
float	g_fCutLine_Dir;

//////////////////////////////////////////////////////

// 픽셀 인 아웃
struct PS_IN
{
	float2		vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

// 픽셀 쉐이더
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(EffectSampler, In.vTexUV);
	vector vBright = tex2D(EffectBlurSampler, In.vTexUV);
	
	///////////////////////////////////////////////////////////////////
	// 블러

	vector vBlur = (vector)0;


	///////////////////////////////////////////////////////////////////

	//int		iCount = 6;//(13)
	//float		g_fRange = 2.4f;
	//float		g_fPower = 0.1f;
	//float		fVar = 0.168f

	int			iCount	= 12;//(25)
	float		fRange	= 0.6f;
	float		fPower	= 0.05f;
	float		fDiv	= 0.084f;

	float		tu = fRange / 640.f;
	float		tv = fRange / 360.f;

	// x축 블러 & y축 블러
	for (int i = -iCount; i < iCount; ++i)
	{
		for (int j = -iCount; j < iCount; ++j)
		{
			float fWeight = (Weight[iCount + i] * 0.2f + 0.5f) * fPower;
			vBlur += fWeight * tex2D(EffectBlurSampler, In.vTexUV + float2(tu * fRange * i  * 2.f, tv));
	
	
			vBlur += fWeight * tex2D(EffectBlurSampler, In.vTexUV + float2(tu, tv * fRange * j));
		}
	}

	vBlur *= fDiv;

	/////////////////////////////////////////////////////////////////
	

	// ViewCut 일때 출력
	if (g_fCutLine_Dir * In.vTexUV.x >= g_fCutLine_Dir * (g_fCutLine_Begin + (g_fCutLine_End - g_fCutLine_Begin) * In.vTexUV.y))
	{
		// 평상시엔 상시 출력
		Out.vColor = vColor + vBlur;
	}

	return Out;
}

technique Default_Device
{
	pass MAIN
	{
		ZEnable = false;
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
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};