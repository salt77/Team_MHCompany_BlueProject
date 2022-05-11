/*			옵션						*/
bool		g_bAntialiasing			= false;
float		FXAA_SPAN_MAX			= 8.0;
float		FXAA_REDUCE_MUL			= 1.0 / 8.0;
float		FXAA_REDUCE_MIN			= 1.0 / 128.0;
float2		frameBufSize			= float2(1280.f, 720.f);
bool		g_bTest					= false;
float		g_fTestRatio			= 0.f;


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

	//mipfilter = none;
	//AddressU = clamp;
	//AddressV = clamp;
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

/*			Depth 텍스쳐			*/
texture		g_DepthTexture;
sampler		DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

//////////////////////////////////////////////////////

bool g_bHDR = true;
float g_fLuminance = 0.08f;
float g_fMiddleGray = 0.18f;
float g_fWhiteCutoff = 0.9f;

bool g_bBloom = true;
float g_fBrightFilter = 0.92f;

//////////////////////////////////////////////////////

// 픽셀 인 아웃
struct PS_IN
{
	float2		vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor		:	COLOR0;
	vector		vLight		:	COLOR1;
	vector		vColorPlus	:	COLOR2;
};

//// 픽셀 쉐이더
//PS_OUT		PS_MAIN(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//	
//	vector vAlbedo = tex2D(AlbedoSampler, In.vTexUV);	
//	vector vEmission = tex2D(EmissionSampler, In.vTexUV);
//	vector vShade = tex2D(ShadeSampler, In.vTexUV);
//	vector vSpecular = tex2D(SpecularSampler, In.vTexUV);
//	
//	////////////////////////////////////////////////////////////////////////
//	// Antialiasing
//	
//	if (g_bTest)
//	{
//		if (In.vTexUV.x < g_fTestRatio)
//		{
//			if (g_bAntialiasing)
//			{
//				// 1번
//				float3 rgbNW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, -1.0) / frameBufSize)).xyz;
//				float3 rgbNE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize)).xyz;
//				float3 rgbSW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize)).xyz;
//				float3 rgbSE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize)).xyz;
//				float3 rgbM = tex2D(AlbedoSampler, In.vTexUV).xyz;
//
//				float3 luma = float3(0.299, 0.587, 0.114);
//				float lumaNW = dot(rgbNW, luma);
//				float lumaNE = dot(rgbNE, luma);
//				float lumaSW = dot(rgbSW, luma);
//				float lumaSE = dot(rgbSE, luma);
//				float lumaM = dot(rgbM, luma);
//
//				float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
//				float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
//
//				float2 dir;
//				dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
//				dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
//
//				float dirReduce = max(
//					(lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
//					FXAA_REDUCE_MIN);
//
//				float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
//
//				dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) / frameBufSize;
//
//				float3 rgbA = (1.0 / 2.0) * (
//					tex2D(AlbedoSampler, In.vTexUV.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
//					tex2D(AlbedoSampler, In.vTexUV.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
//				float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
//					tex2D(AlbedoSampler, In.vTexUV.xy + dir * (0.0 / 3.0 - 0.5)).xyz +
//					tex2D(AlbedoSampler, In.vTexUV.xy + dir * (3.0 / 3.0 - 0.5)).xyz);
//				float lumaB = dot(rgbB, luma);
//
//				if ((lumaB < lumaMin) || (lumaB > lumaMax))
//				{
//					vAlbedo.xyz = rgbA;
//				}
//				else
//				{
//					vAlbedo.xyz = rgbB;
//				}
//			}
//
//			////////////////////////////////////////////////////////////////////////
//			// hdr
//
//			if (g_bHDR)
//			{
//				float fHDR = 2.2f;
//				float3 Color = vAlbedo.rgb;
//				Color = pow(abs(Color), fHDR) * g_fMiddleGray / (g_fLuminance + 0.001f);
//
//				Color *= (Color / (g_fWhiteCutoff * g_fWhiteCutoff)) + 1.f;
//				Color /= Color + 1.f;
//
//				Color *= vShade.rgb;
//
//				Color = pow(abs(Color), 1.f / fHDR);
//
//				vAlbedo.rgb = Color;
//
//			}
//			else
//			{
//				vAlbedo *= vShade;
//			}
//
//			//vSpecular = 0.f;
//			Out.vColor = vAlbedo + vEmission + vSpecular;
//
//			////////////////////////////////////////////////////////////////////////
//			// Bright Filter
//
//			float BrightColor = 0.f;
//			if (g_bBloom)
//			{
//				float4 FragColor = tex2D(AlbedoSampler, In.vTexUV);
//
//				float brightness = dot(FragColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
//
//				if (brightness > g_fBrightFilter)
//				{
//					BrightColor = float4(FragColor.rgb, 1.f);
//				}
//			}
//			Out.vLight = BrightColor + vEmission + vSpecular;
//		}
//		else
//		{
//			float BrightColor = 0.f;
//			// 아무후처리도 안함
//			vAlbedo *= vShade;
//			Out.vColor = vAlbedo + vEmission + vSpecular;
//			Out.vLight = BrightColor + vEmission + vSpecular;
//		}
//	}
//	else
//	{
//		if (g_bAntialiasing)
//		{
//			// 1번
//			float3 rgbNW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, -1.0) / frameBufSize)).xyz;
//			float3 rgbNE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize)).xyz;
//			float3 rgbSW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize)).xyz;
//			float3 rgbSE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize)).xyz;
//			float3 rgbM = tex2D(AlbedoSampler, In.vTexUV).xyz;
//
//			float3 luma = float3(0.299, 0.587, 0.114);
//			float lumaNW = dot(rgbNW, luma);
//			float lumaNE = dot(rgbNE, luma);
//			float lumaSW = dot(rgbSW, luma);
//			float lumaSE = dot(rgbSE, luma);
//			float lumaM = dot(rgbM, luma);
//
//			float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
//			float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
//
//			float2 dir;
//			dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
//			dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
//
//			float dirReduce = max(
//				(lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
//				FXAA_REDUCE_MIN);
//
//			float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
//
//			dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) / frameBufSize;
//
//			float3 rgbA = (1.0 / 2.0) * (
//				tex2D(AlbedoSampler, In.vTexUV.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
//				tex2D(AlbedoSampler, In.vTexUV.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
//			float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
//				tex2D(AlbedoSampler, In.vTexUV.xy + dir * (0.0 / 3.0 - 0.5)).xyz +
//				tex2D(AlbedoSampler, In.vTexUV.xy + dir * (3.0 / 3.0 - 0.5)).xyz);
//			float lumaB = dot(rgbB, luma);
//
//			if ((lumaB < lumaMin) || (lumaB > lumaMax))
//			{
//				vAlbedo.xyz = rgbA;
//			}
//			else
//			{
//				vAlbedo.xyz = rgbB;
//			}
//		}
//
//	////////////////////////////////////////////////////////////////////////
//	// hdr
//	
//		if (g_bHDR)
//		{			
//			float fHDR = 2.2f;
//			float3 Color = vAlbedo.rgb;
//			Color = pow(abs(Color), fHDR) * g_fMiddleGray / (g_fLuminance + 0.001f);
//			
//			Color *= (Color / (g_fWhiteCutoff * g_fWhiteCutoff)) + 1.f;
//			Color /= Color + 1.f;
//			
//			Color *= vShade.rgb;
//			
//			Color = pow(abs(Color), 1.f / fHDR);
//			
//			vAlbedo.rgb = Color;
//		}
//		else
//		{
//			vAlbedo *= vShade;
//		}
//
//		vSpecular = 0.f;
//		Out.vColor = vAlbedo + vEmission + vSpecular;
//
//		////////////////////////////////////////////////////////////////////////
//		// Bright Filter
//
//		float BrightColor = 0.f;
//		if (g_bBloom)
//		{
//			float4 FragColor = tex2D(AlbedoSampler, In.vTexUV);
//
//			float brightness = dot(FragColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
//
//			if (brightness > g_fBrightFilter)
//			{
//				BrightColor = float4(FragColor.rgb, 1.f);
//			}
//		}
//		Out.vLight = BrightColor + vEmission + vSpecular;
//	}
//	
//	
//	return Out;
//}
//
//
//// 흰색으로 그려지는 거 
//PS_OUT		PS_MAIN_ANTI(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//
//	vector vAlbedo_M  = tex2D(AlbedoSampler, In.vTexUV);
//	vector vAlbedo_NW = tex2D(AlbedoSampler, In.vTexUV	+ (float2(-1.0, -1.0) / frameBufSize));
//	vector vAlbedo_NE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize));
//	vector vAlbedo_SW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize));
//	vector vAlbedo_SE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize));
//
//	vector vEmission_M  = tex2D(EmissionSampler, In.vTexUV);
//	vector vEmission_NW = tex2D(EmissionSampler, In.vTexUV + (float2(-1.0, -1.0) / frameBufSize));
//	vector vEmission_NE = tex2D(EmissionSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize));
//	vector vEmission_SW = tex2D(EmissionSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize));
//	vector vEmission_SE = tex2D(EmissionSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize));
//
//	vector vShade_M  = tex2D(ShadeSampler, In.vTexUV);
//	vector vShade_NW = tex2D(ShadeSampler, In.vTexUV + (float2(-1.0, -1.0) / frameBufSize));
//	vector vShade_NE = tex2D(ShadeSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize));
//	vector vShade_SW = tex2D(ShadeSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize));
//	vector vShade_SE = tex2D(ShadeSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize));
//
//	float3 rgbNW	= (vAlbedo_M	* vShade_M		+ vEmission_M).xyz;
//	float3 rgbNE	= (vAlbedo_NW	* vShade_NW		+ vEmission_NW).xyz;
//	float3 rgbSW	= (vAlbedo_NE	* vShade_NE		+ vEmission_NE).xyz;
//	float3 rgbSE	= (vAlbedo_SW	* vShade_SW		+ vEmission_SW).xyz;
//	float3 rgbM		= (vAlbedo_SE	* vShade_SE		+ vEmission_SE).xyz;
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
//	dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) / frameBufSize;
//
//	float2 fA_1 = In.vTexUV.xy + dir * (1.0 * 0.33 - 0.5);
//	float2 fA_2 = In.vTexUV.xy + dir * (2.0 * 0.33 - 0.5);
//	float2 fB_1 = In.vTexUV.xy + dir * (0.0 * 0.33 - 0.5);
//	float2 fB_2 = In.vTexUV.xy + dir * (3.0 * 0.33 - 0.5);
//
//	vector vAlbedo_RGB_A_1 = tex2D(AlbedoSampler, fA_1);
//	vector vAlbedo_RGB_A_2 = tex2D(AlbedoSampler, fA_2);
//	vector vAlbedo_RGB_B_1 = tex2D(AlbedoSampler, fB_1);
//	vector vAlbedo_RGB_B_2 = tex2D(AlbedoSampler, fB_2);
//
//	vector vEmission_RGB_A_1 = tex2D(EmissionSampler, fA_1);
//	vector vEmission_RGB_A_2 = tex2D(EmissionSampler, fA_2);
//	vector vEmission_RGB_B_1 = tex2D(EmissionSampler, fB_1);
//	vector vEmission_RGB_B_2 = tex2D(EmissionSampler, fB_2);
//
//	vector vShade_RGB_A_1 = tex2D(ShadeSampler, fA_1);
//	vector vShade_RGB_A_2 = tex2D(ShadeSampler, fA_2);
//	vector vShade_RGB_B_1 = tex2D(ShadeSampler, fB_1);
//	vector vShade_RGB_B_2 = tex2D(ShadeSampler, fB_2);
//
//	float3 RGB_A_1 = (vAlbedo_RGB_A_1	* vEmission_RGB_A_1 + vShade_RGB_A_1).xyz;
//	float3 RGB_A_2 = (vAlbedo_RGB_A_2	* vEmission_RGB_A_2 + vShade_RGB_A_2).xyz;
//	float3 RGB_B_1 = (vAlbedo_RGB_B_1	* vEmission_RGB_B_1 + vShade_RGB_B_1).xyz;
//	float3 RGB_B_2 = (vAlbedo_RGB_B_2	* vEmission_RGB_B_2 + vShade_RGB_B_2).xyz;
//
//
//	float3 rgb_AAA = (1.0 / 2.0) * (RGB_A_1 + RGB_A_2);
//	float3 rgb_BBB = rgb_AAA * (1.0 / 2.0) + (1.0 / 4.0) * (RGB_B_1 + RGB_B_2);
//	float lumaB = dot(rgb_BBB, luma);
//
//	if ((lumaB < lumaMin) || (lumaB > lumaMax))
//	{
//		Out.vColor = vector(rgb_AAA, 1.f);
//	}
//	else
//	{
//		Out.vColor = vector(rgb_BBB, 1.f);
//	}
//
//	///Out.vColor.a = 1.f;
//
//	return Out;
//}


// 최종
PS_OUT		PS_Result(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector vEmission = tex2D(EmissionSampler, In.vTexUV);
	vector vShade = tex2D(ShadeSampler, In.vTexUV);
	//vector vSpecular = tex2D(SpecularSampler, In.vTexUV);

	////////////////////////////////////////////////////////////////////////
	// 안티 엘리어싱

	float3 rgbNW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, -1.0) / frameBufSize)).xyz;
	float3 rgbNE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, -1.0) / frameBufSize)).xyz;
	float3 rgbSW = tex2D(AlbedoSampler, In.vTexUV + (float2(-1.0, 1.0) / frameBufSize)).xyz;
	float3 rgbSE = tex2D(AlbedoSampler, In.vTexUV + (float2(1.0, 1.0) / frameBufSize)).xyz;
	float3 rgbM = tex2D(AlbedoSampler, In.vTexUV).xyz;

	float3 luma = float3(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM = dot(rgbM, luma);

	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

	float2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

	float dirReduce = max(
		(lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
		FXAA_REDUCE_MIN);

	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) / frameBufSize;

	float3 rgbA = (1.0 / 2.0) * (
		tex2D(AlbedoSampler, In.vTexUV.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
		tex2D(AlbedoSampler, In.vTexUV.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
	float3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * (
		tex2D(AlbedoSampler, In.vTexUV.xy + dir * (0.0 / 3.0 - 0.5)).xyz +
		tex2D(AlbedoSampler, In.vTexUV.xy + dir * (3.0 / 3.0 - 0.5)).xyz);
	float lumaB = dot(rgbB, luma);

	if ((lumaB < lumaMin) || (lumaB > lumaMax))
	{
		vAlbedo.xyz = rgbA;
	}
	else
	{
		vAlbedo.xyz = rgbB;
	}

	////////////////////////////////////////////////////////////////////////
	// hdr

	if (g_bHDR)
	{
		float fHDR = 2.2f;
		float3 Color = vAlbedo.rgb;
		Color = pow(abs(Color), fHDR) * g_fMiddleGray / (g_fLuminance + 0.001f);

		Color *= (Color / (g_fWhiteCutoff * g_fWhiteCutoff)) + 1.f;
		Color /= Color + 1.f;

		Color *= vShade.rgb;

		Color = pow(abs(Color), 1.f / fHDR);

		vAlbedo.rgb = Color;
	}
	else
	{
		vAlbedo *= vShade;
	}

	Out.vColor = vAlbedo + vEmission;
	//Out.vColor.a = 1.f;

	////////////////////////////////////////////////////////////////////////
	// Bright Filter

	float BrightColor = 0.f;
	if (g_bBloom)
	{
		float4 FragColor = tex2D(AlbedoSampler, In.vTexUV);

		float brightness = dot(FragColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));

		if (brightness > g_fBrightFilter)
		{
			BrightColor = float4(FragColor.rgb, 1.f);
		}
	}
	Out.vLight = BrightColor + vEmission;
	   	 
	////////////////////////////////////////////////////////////////////////
	// 색상 보정

	// 마스킹된 객체만 HDR후에 색상 보정을 하겠다 
	Out.vColorPlus = vector(1.f, 1.f, 1.f, 1.f);

	if (0.f < tex2D(DepthSampler, In.vTexUV).b)
	{
		Out.vColorPlus.rgb = tex2D(AlbedoSampler, In.vTexUV).rgb;
	}
	   
	return Out;
}

technique Default_Device
{
	//pass MAIN
	//{
	//	zwriteenable = false;
	//
	//	AlphaBlendEnable = true;
	//	SRCBLEND = SRCALPHA;
	//	DESTBLEND = INVSRCALPHA;
	//	
	//	AlphaTestEnable = false;
	//
	//	//AlphaTestEnable = false;
	//	//
	//	//AlphaTestEnable = true;
	//	//AlphaFunc = Greater;
	//	//AlphaRef = 1;
	//
	//	vertexshader = NULL;
	//	pixelshader = compile ps_3_0 PS_MAIN();
	//}
	//
	//pass MAIN_2
	//{
	//	zwriteenable = false;
	//	AlphaTestEnable = true;
	//
	//	AlphaFunc = Greater;
	//	AlphaRef = 1;
	//
	//	vertexshader = NULL;
	//	pixelshader = compile ps_3_0 PS_MAIN_ANTI();
	//}

	pass Result
	{
		ZWriteEnable = false;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;

		//AlphaTestEnable = false;
		//
		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 1;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_Result();
	}
};