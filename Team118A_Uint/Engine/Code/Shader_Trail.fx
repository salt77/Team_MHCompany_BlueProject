
//														상수

matrix		g_matWorld;
matrix		g_matView;
matrix		g_matProj;

float		g_fLifeRT;

bool		g_bBlackMask;
bool		g_bFadeOut_Alpha;
bool		g_bUV_X_Inverse;

//														텍스쳐
texture g_TrailTexture;
sampler TrailSampler = sampler_state
{
	texture = g_TrailTexture;
};
texture g_Filter01Texture;
sampler Filter01Sampler = sampler_state
{
	texture = g_Filter01Texture;
};






struct VS_IN
{
	vector			vPosition	:	POSITION0;			// 시멘틱 : 정점의 구성 옵션(쉽게 말해 FVF)
	float2			vTexUV		:	TEXCOORD0;
	float2			vRatio		:	TEXCOORD1;
};

struct VS_OUT
{
	vector			vPosition	:	POSITION;
	float2			vTexUV		:	TEXCOORD0;
	float2			vRatio		:	TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

VS_OUT			VS_TRAIL(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matVP;
	matVP = mul(g_matView, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matVP);

	Out.vTexUV = In.vTexUV;
	Out.vRatio = In.vRatio;
	return Out;
}

PS_OUT		PS_TRAIL(VS_OUT In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vTrail = tex2D(TrailSampler, In.vTexUV);
	vector vFilter = tex2D(Filter01Sampler, In.vTexUV);

	if (g_bBlackMask)
	{
		if (vTrail.x + vTrail.y + vTrail.z > 0.001f)
		{
			Out.vColor = vTrail;
		}
		else
		{
			Out.vColor = 0;
		}
	}
	else
	{
		Out.vColor = vTrail;
	}

	if (g_bFadeOut_Alpha)
	{
		int iIntX = (int)In.vTexUV.x;

		if (g_bUV_X_Inverse)
		{
			Out.vColor.a = (In.vTexUV.x - iIntX);
		}
		else
		{
			Out.vColor.a = 1.f - (In.vTexUV.x - iIntX);
		}
	}
	return Out;
}

PS_OUT		PS_TRAIL2(VS_OUT In)
{
	PS_OUT		Out = (PS_OUT)0;
	return Out;
}

technique Default_Device
{
	pass Normal		/*------- Index 0 -------*/
	{
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		alphatestenable = false;

		CullMode = false;

		vertexshader = compile vs_3_0 VS_TRAIL();
		pixelshader = compile ps_3_0 PS_TRAIL();
	}

	pass FireSword	/*------- Index 1 -------*/
	{
		alphatestenable = true;
		alphafunc = greater;
		alpharef = 1;

		AlphaBlendEnable = false;

		CullMode = false;

		vertexshader = compile vs_3_0 VS_TRAIL();
		pixelshader = compile ps_3_0 PS_TRAIL2();
	}
};