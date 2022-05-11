
/*			옵션						*/
bool		g_bAntialiasing = false;
float		FXAA_SPAN_MAX = 8.0;
float		FXAA_REDUCE_MUL = 1.0 / 8.0;
float		FXAA_REDUCE_MIN = 1.0 / 128.0;
float2		frameBufSize = float2(1280.f, 720.f);

/*			Blend 텍스쳐			*/
texture		g_BlendTexture;
sampler		BlendSampler = sampler_state
{
	texture = g_BlendTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

/*			Blur 텍스쳐			*/
texture		g_LightTexture;
sampler		LightSampler = sampler_state
{
	texture = g_LightTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

/*			GodRay 텍스쳐			*/
texture		g_GodRayTexture;
sampler		GodRaySampler = sampler_state
{
	texture = g_GodRayTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

/*			Color 보정 텍스쳐			*/
texture		g_ColorTexture;
sampler		ColorSampler = sampler_state
{
	texture = g_ColorTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	AddressU = clamp;
	AddressV = clamp;
};

// CutLine
float	g_fCutLine_Begin;
float	g_fCutLine_End;
float	g_fCutLine_Dir;
vector	g_vCutColor;

float	g_fFixColor;

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
	
	vector vBlend = tex2D(BlendSampler, In.vTexUV);
	vector vBlur = tex2D(LightSampler, In.vTexUV);
	vector vRay = tex2D(GodRaySampler, In.vTexUV);
	vector vFixColor = tex2D(ColorSampler, In.vTexUV);

	vFixColor.rgb = saturate(vFixColor.rgb + g_fFixColor);

	vector vColor = vBlend * vFixColor + vBlur + vRay;
	
	// ViewCut 일때 출력
	if (g_fCutLine_Dir * In.vTexUV.x >= g_fCutLine_Dir * (g_fCutLine_Begin + (g_fCutLine_End - g_fCutLine_Begin) * In.vTexUV.y))
	{
		// 평상시엔 상시 출력
		Out.vColor = vColor;
	}
	
	return Out;








	//vector vColor = vBlend;
	//if (g_bAntialiasing)
	//{
	//	// 1번
	//	float3 rgbNW = tex2D(BlendSampler, In.vTexUV + (float2(-1.0, -1.0) / frameBufSize)).xyz;
	//	float3 rgbNE = tex2D(BlendSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize)).xyz;
	//	float3 rgbSW = tex2D(BlendSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize)).xyz;
	//	float3 rgbSE = tex2D(BlendSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize)).xyz;
	//	float3 rgbM = tex2D(BlendSampler, In.vTexUV).xyz;
	//
	//	float3 luma = float3(0.299, 0.587, 0.114);
	//	float lumaNW = dot(rgbNW, luma);
	//	float lumaNE = dot(rgbNE, luma);
	//	float lumaSW = dot(rgbSW, luma);
	//	float lumaSE = dot(rgbSE, luma);
	//	float lumaM = dot(rgbM, luma);
	//
	//	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	//	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	//
	//	float2 dir;
	//	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	//	dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
	//
	//	float dirReduce = max(
	//		(lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
	//		FXAA_REDUCE_MIN);
	//
	//	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
	//
	//	dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
	//		max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
	//			dir * rcpDirMin)) / frameBufSize;
	//
	//	float3 rgbA = (1.0 / 2.0) * (
	//		tex2D(BlendSampler, In.vTexUV.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
	//		tex2D(BlendSampler, In.vTexUV.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
	//	float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
	//		tex2D(BlendSampler, In.vTexUV.xy + dir * (0.0 / 3.0 - 0.5)).xyz +
	//		tex2D(BlendSampler, In.vTexUV.xy + dir * (3.0 / 3.0 - 0.5)).xyz);
	//	float lumaB = dot(rgbB, luma);
	//
	//	if ((lumaB < lumaMin) || (lumaB > lumaMax))
	//	{
	//		vColor.xyz = rgbA;
	//	}
	//	else
	//	{
	//		vColor.xyz = rgbB;
	//	}
	//}

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

		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 1;


		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};