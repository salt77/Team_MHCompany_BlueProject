

// �⺻ �ɼ�
matrix	g_matWorld, g_matView, g_matProj;
vector	g_vColor;

float	g_fU;
float	g_fV;
float	g_fU_Scale;
float	g_fV_Scale;

////////////////////////////////////////////
// �߰��ɼ�

float	g_bFilter_1;
float	g_bFilter_2;
float	g_bFilter_3;

float	g_bBlackAlpha;
float	g_bBlackWhite;

////////////////////////////////////////////
// ����_1

float	g_fRatio_1;

float	g_fMin_Filter_1;
float	g_fMax_Filter_1;

float	g_fR_Filter_1;
float	g_fG_Filter_1;
float	g_fB_Filter_1;

////////////////////////////////////////////
// ����_2

float	g_fRatio_2;

float	g_fMin_Filter_2;
float	g_fMax_Filter_2;

float	g_fR_Filter_2;
float	g_fG_Filter_2;
float	g_fB_Filter_2;

////////////////////////////////////////////
// ����_4

float	g_fR_Filter_4;
float	g_fG_Filter_4;
float	g_fB_Filter_4;
float	g_fA_Filter_4;
float	g_fU_Filter_4;
float	g_fV_Filter_4;

////////////////////////////////////////////

// Albedo �ؽ���
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

// filter_1 �ؽ���
texture	g_FilterTexture_1;
sampler	FilterSampler_1 = sampler_state
{
	texture = g_FilterTexture_1;

	minfilter = linear;
	magfilter = linear;
};

// filter_2 �ؽ���
texture	g_FilterTexture_2;
sampler	FilterSampler_2 = sampler_state
{
	texture = g_FilterTexture_2;

	minfilter = linear;
	magfilter = linear;
};

// filter_3 �ؽ���
texture	g_FilterTexture_3;
sampler	FilterSampler_3 = sampler_state
{
	texture = g_FilterTexture_3;

	minfilter = linear;
	magfilter = linear;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���ؽ� �� �ƿ�
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

// ���ؽ� ���̴�
VS_OUT		VS_BASE(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	
	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��ƼŬ ���ؽ� �� �ƿ�
struct VS_PARTICLE_IN
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector		vColor		: COLOR0;
	vector		vRight		: COLOR1;
	vector		vUp			: COLOR2;
	vector      vLook		: COLOR3;
	vector		vPos		: COLOR4;
	vector		vSpriteUV   : COLOR5;
};

struct VS_PARTICLE_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector		vColor		: COLOR0;
	vector      vSpriteUV   : COLOR1;
};

// ��ƼŬ ���ؽ� ���̴�
VS_PARTICLE_OUT		VS_PARTICLE_BASE(VS_PARTICLE_IN In)
{
	VS_PARTICLE_OUT Out = (VS_PARTICLE_OUT)0;

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
	Out.vTexUV = In.vTexUV;
	Out.vColor = In.vColor;
	Out.vSpriteUV = In.vSpriteUV;
	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �ȼ� �ƿ�
struct PS_OUT
{
	vector		vColor		:	COLOR0;
	vector		vEmissive	:	COLOR1;
};

// �ȼ� ���̴�
PS_OUT		PS_BASE(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x += g_fU;
	vUV.y += g_fV;
	vUV.x *= g_fU_Scale;
	vUV.y *= g_fV_Scale;


	//// �ؽ�ó ���� ����
	//vector vColor = g_vColor;
	//vector vTexColor = tex2D(BaseSampler, vUV);	
	//float fGray = (vTexColor.r + vTexColor.g + vTexColor.b) * 0.33f;

	//// �ؽ�ó �̹����� ������� �����.
	//if (g_bBlackWhite)
	//{
	//	vTexColor = vector(fGray, fGray, fGray, vTexColor.a);
	//}
	//
	//// ���� ���� ����� �̹����� ���
	//if (g_bBlackAlpha) 
	//{
	//	vColor.a *= fGray;
	//}
	//else 
	//{
	//	vColor *= vTexColor;
	//}

	// �ؽ�ó ���� ����
	vector vColor = g_vColor;
	vector vTexColor = tex2D(BaseSampler, vUV);	
	float fGray = (vTexColor.r + vTexColor.g + vTexColor.b) * 0.33f;

	// �ؽ�ó �̹����� ������� �����.
	if (g_bBlackWhite)
	{
		vTexColor.rgb = fGray;
	}
	
	// ���� ���� ����� �̹����� ���
	if (g_bBlackAlpha) 
	{
		vTexColor.a *= fGray;
	}

	vColor = vTexColor * vColor;

	// ����_1
	if (g_bFilter_1) 
	{
		float3 vFilter = tex2D(FilterSampler_1, In.vTexUV).rgb;
		float fFilter_1 = (vFilter.r * g_fR_Filter_1 + vFilter.g * g_fG_Filter_1 + vFilter.b * g_fB_Filter_1) 
							/ (g_fR_Filter_1 + g_fG_Filter_1 + g_fB_Filter_1);
		
		float fRatio_1 = (fFilter_1 - g_fMin_Filter_1) / (g_fMax_Filter_1 - g_fMin_Filter_1);

		if (fRatio_1 < g_fRatio_1)
		{
			vColor.a = 0.f;
		}
	}
	
	// ����_2
	if (g_bFilter_2)
	{
		float3 vFilter = tex2D(FilterSampler_2, In.vTexUV).rgb;
		float fFilter_2 = (vFilter.r * g_fR_Filter_2 + vFilter.g * g_fG_Filter_2 + vFilter.b * g_fB_Filter_2)
			/ (g_fR_Filter_2 + g_fG_Filter_2 + g_fB_Filter_2);

		float fRatio_2 = (fFilter_2 - g_fMin_Filter_2) / (g_fMax_Filter_2 - g_fMin_Filter_2);

		if (fRatio_2 < g_fRatio_2)
		{
			vColor.a = 0.f;
		}
	}

	// ����_3 Į�� ����
	if (g_bFilter_3)
	{
		float4 vImage_filter4 = tex2D(FilterSampler_3, float2(vUV.x + g_fU_Filter_4, vUV.y + g_fV_Filter_4));
		float3 vColor_filter4 = float3(vImage_filter4.r * g_fR_Filter_4, 
										vImage_filter4.g * g_fG_Filter_4, 
										vImage_filter4.b * g_fB_Filter_4);

		vColor.rgb = vColor.rgb * (1.f - g_fA_Filter_4) + vColor_filter4 * g_fA_Filter_4;
	}

	Out.vColor = vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �ȼ� ���̴�
PS_OUT		PS_BLUR(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vUV = In.vTexUV;
	vUV.x += g_fU;
	vUV.y += g_fV;
	vUV.x *= g_fU_Scale;
	vUV.y *= g_fV_Scale;


	//// �ؽ�ó ���� ����
	//vector vColor = g_vColor;
	//vector vTexColor = tex2D(BaseSampler, vUV);	
	//float fGray = (vTexColor.r + vTexColor.g + vTexColor.b) * 0.33f;

	//// �ؽ�ó �̹����� ������� �����.
	//if (g_bBlackWhite)
	//{
	//	vTexColor = vector(fGray, fGray, fGray, vTexColor.a);
	//}
	//
	//// ���� ���� ����� �̹����� ���
	//if (g_bBlackAlpha) 
	//{
	//	vColor.a *= fGray;
	//}
	//else 
	//{
	//	vColor *= vTexColor;
	//}

	// �ؽ�ó ���� ����
	vector vColor = g_vColor;
	vector vTexColor = tex2D(BaseSampler, vUV);
	float fGray = (vTexColor.r + vTexColor.g + vTexColor.b) * 0.33f;

	// �ؽ�ó �̹����� ������� �����.
	if (g_bBlackWhite)
	{
		vTexColor.rgb = fGray;
	}

	// ���� ���� ����� �̹����� ���
	if (g_bBlackAlpha)
	{
		vTexColor.a *= fGray;
	}

	vColor = vTexColor * vColor;

	// ����_1
	if (g_bFilter_1)
	{
		float3 vFilter = tex2D(FilterSampler_1, In.vTexUV).rgb;
		float fFilter_1 = (vFilter.r * g_fR_Filter_1 + vFilter.g * g_fG_Filter_1 + vFilter.b * g_fB_Filter_1)
			/ (g_fR_Filter_1 + g_fG_Filter_1 + g_fB_Filter_1);

		float fRatio_1 = (fFilter_1 - g_fMin_Filter_1) / (g_fMax_Filter_1 - g_fMin_Filter_1);

		if (fRatio_1 < g_fRatio_1)
		{
			vColor.a = 0.f;
		}
	}

	// ����_2
	if (g_bFilter_2)
	{
		float3 vFilter = tex2D(FilterSampler_2, In.vTexUV).rgb;
		float fFilter_2 = (vFilter.r * g_fR_Filter_2 + vFilter.g * g_fG_Filter_2 + vFilter.b * g_fB_Filter_2)
			/ (g_fR_Filter_2 + g_fG_Filter_2 + g_fB_Filter_2);

		float fRatio_2 = (fFilter_2 - g_fMin_Filter_2) / (g_fMax_Filter_2 - g_fMin_Filter_2);

		if (fRatio_2 < g_fRatio_2)
		{
			vColor.a = 0.f;
		}
	}

	// ����_3 Į�� ����
	if (g_bFilter_3)
	{
		float4 vImage_filter4 = tex2D(FilterSampler_3, float2(vUV.x + g_fU_Filter_4, vUV.y + g_fV_Filter_4));
		float3 vColor_filter4 = float3(vImage_filter4.r * g_fR_Filter_4,
			vImage_filter4.g * g_fG_Filter_4,
			vImage_filter4.b * g_fB_Filter_4);

		vColor.rgb = vColor.rgb * (1.f - g_fA_Filter_4) + vColor_filter4 * g_fA_Filter_4;
	}

	Out.vColor = vColor;
	Out.vEmissive = vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ƼŬ �ȼ� ���̴�
PS_OUT		PS_PARTICLE_BASE(VS_PARTICLE_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vUV;

	vUV.x = In.vSpriteUV.x + In.vSpriteUV.z * In.vTexUV.x;
	vUV.y = In.vSpriteUV.y + In.vSpriteUV.w * In.vTexUV.y;
	vector vColor = tex2D(BaseSampler, vUV);


	Out.vColor = vColor;

	if (g_bBlackAlpha)
	{
		vColor.a = vColor.r;
	}
	vColor *= In.vColor;

	Out.vColor = vColor;

	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//��ƼŬ �ȼ� ���̴� ��
PS_OUT		PS_PARTICLE_BLUR(VS_PARTICLE_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vUV;

	vUV.x = In.vSpriteUV.x + In.vSpriteUV.z * In.vTexUV.x;
	vUV.y = In.vSpriteUV.y + In.vSpriteUV.w * In.vTexUV.y;
	vector vColor = tex2D(BaseSampler, vUV);

	Out.vColor = vColor;

	if (g_bBlackAlpha)
	{
		vColor.a = vColor.r;
	}
	vColor *= In.vColor;

	Out.vColor = vColor;
	Out.vEmissive = vColor;

	return Out;
}

//	//
//	struct VS_INPUT
//	{
//		float4 pos : POSITION;
//		float4 diff : COLOR0;
//		float2 uv : TEXCOORD0;//
//	};/
//	struct VS_OUTPUT
//	{
//		float4 pos : POSITION;
//		float4 diff : COLOR0;
//		float2 uv : TEXCOORD0;//
//	};
//	VS_OUTPUT _VS(VS_INPUT _In)
//	{
//		VS_OUTPUT Out = (VS_OUTPUT)0;
//
//		Out.pos = _In.pos;
//		Out.uv = _In.uv;
//		Out.diff = _In.diff;
//
//
//		return Out;
//	}
//	float4 _PS_Distortion(VS_OUTPUT _In) : COLOR
//	{
//		float4 Out = 0;
//
//		//uv trans
//		// 0.001f �� �׽�Ʈ�� �־��� ���̸�, tick, timer ���� ������ ��ü�ϸ� �˴ϴ�.
//		float2 Trans = _In.uv + 0.001f;

//		float4 Noise = tex2D(Samp_Texture_Distortion, Trans);
//
//		//������������ UV �� ���� �ֱ� ���� ���
//		// 0.05 �� �� �κ��� Ŀ������ UV ������ ���� �������ϴ�.
//		float2 UV = _In.uv + Noise.xy * 0.05f;
//
//		//
//		float4 Orig = tex2D(Samp_Texture_Orig, UV);
//
//
//		Out = Orig;
//
//		return Out;
//	}
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �н�
technique Default_Device
{
	pass Base
	{
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_BASE();
		pixelshader = compile ps_3_0 PS_BASE();
	}

	pass Blur
	{
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_BASE();
		pixelshader = compile ps_3_0 PS_BLUR();
	}

	pass ParticleBase
	{
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_PARTICLE_BASE();
		pixelshader = compile ps_3_0 PS_PARTICLE_BASE();
	}

	pass ParticleBlur
	{
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_PARTICLE_BASE();
		pixelshader = compile ps_3_0 PS_PARTICLE_BLUR();
	}

};