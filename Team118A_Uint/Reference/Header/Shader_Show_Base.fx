

// �⺻ ���
matrix	g_matWorld, g_matView, g_matProj;

// Albedo �ؽ���
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

// Mask �ؽ���
// ����ũ�� �̿��� �۷ο�
float	g_fTexGlowR;
float	g_fTexGlowG;
float	g_fTexGlowB;

texture	g_MaskTexture;
sampler	MaskSampler = sampler_state
{
	texture = g_MaskTexture;

	minfilter = linear;
	magfilter = linear;
};

// �۷ο� �ؽ�ó�� uv ���Ѱ�
float	g_fGlow_U_Min;
float	g_fGlow_U_Max;
float	g_fGlow_V_Min;
float	g_fGlow_V_Max;

// ����ũ�� �̿����� �ʴ� �۷ο�
float	g_fBaseGlow;

float	g_fGlowPower;

float	g_fGlowColorR;
float	g_fGlowColorG;
float	g_fGlowColorB;

// DMesh �����
matrix	g_Bone[50];

//��Ű�� ������ �޴� ���� ����
int		g_iNum;

// DMesh ĳ���� ��
bool	g_bMouse = false;
float	g_fMouseU;
float	g_fMouseV;
bool	g_bFace = false;

texture	g_MouseTexture;
sampler	MouseSampler = sampler_state
{
	texture = g_MouseTexture;

	minfilter = linear;
	magfilter = linear;
};

////////////////////////////////////////////////////////////////

struct VS_IN_Color_DMesh
{
	vector		vPosition		: POSITION;
	vector		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;

	float4		vBlendWeights	: BLENDWEIGHT;
	float4		vBlendIndices	: BLENDINDICES;
};

struct VS_OUT_Color_DMesh
{
	vector		vPosition		: POSITION;
	vector		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	vector		vProjPos		: TEXCOORD1;
	vector		vWorldPos		: TEXCOORD2;
};

VS_OUT_Color_DMesh		VS_Color_DMesh(VS_IN_Color_DMesh In)
{
	VS_OUT_Color_DMesh Out = (VS_OUT_Color_DMesh)0;

	///////////////////////////////////////////////////////////////////
	//�ϵ���� ��Ű��

	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);
	float4 vNormal = float4(0.f, 0.f, 0.f, 0.f);
	float fLast = 1.f;

	int4 iIndexArray = D3DCOLORtoUBYTE4(In.vBlendIndices);

	int iNum = g_iNum - 1;
	for (int i = 0; i < iNum; ++i)
	{
		fLast -= In.vBlendWeights[i];
		vPos += mul(In.vPosition, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
		vNormal += mul(In.vNormal, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
	}
	vPos += fLast * mul(In.vPosition, g_Bone[int(In.vBlendIndices[iNum])]);
	vPos.w = 1.f;
	vNormal += fLast * mul(In.vNormal, g_Bone[int(In.vBlendIndices[iNum])]);
	vNormal.w = 0.f;

	///////////////////////////////////////////////////////////////////

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vPos, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = vPos;

	return Out;
}

struct PS_OUT_Color_DMesh
{
	vector		vColor		:	COLOR0;
	vector		vEmission	:	COLOR1;
	vector		vNormal		:	COLOR2;
	vector		vDepth		:	COLOR3;
};

PS_OUT_Color_DMesh		PS_Color_DMesh(VS_OUT_Color_DMesh In)
{
	PS_OUT_Color_DMesh Out = (PS_OUT_Color_DMesh)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);

	// face ����
	if (g_bFace || g_bMouse)
	{
		Out.vDepth.b = 0.5f;
	}

	// Mouse �ؽ�ó

	if (g_bMouse && In.vTexUV.x < 0.25f && In.vTexUV.y > 0.75f)
	{
		Out.vColor = tex2D(MouseSampler, float2(In.vTexUV.x + g_fMouseU, In.vTexUV.y + g_fMouseV - 0.75f));
	}

	// Glow && Mask �ؽ�ó
	if (g_fGlow_U_Min <= In.vTexUV.x && g_fGlow_U_Max > In.vTexUV.x &&
		g_fGlow_V_Min <= In.vTexUV.y && g_fGlow_V_Max > In.vTexUV.y)
	{
		// ���̽� + �ؽ�ó �۷ο� (0.f~1.f)
		float3 vGlow = tex2D(MaskSampler, In.vTexUV).rgb;
		float fGlow = saturate(g_fBaseGlow + vGlow.r * g_fTexGlowR + vGlow.g * g_fTexGlowG + vGlow.b * g_fTexGlowB);

		// �۷ο� ����
		Out.vEmission = Out.vColor * fGlow * g_fGlowPower * float4(g_fGlowColorR, g_fGlowColorG, g_fGlowColorB, 1.f);
	}


	return Out;
}

technique Default_Device
{
	pass Color_DMesh
	{
		ZEnable = True;
		ZWriteEnable = True;

		// ĳ������ �Զ�����... 
		AlphaBlendEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color_DMesh();
		pixelshader = compile ps_3_0 PS_Color_DMesh();
	}
};