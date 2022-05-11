

// �⺻ ���
matrix	g_matWorld, g_matView, g_matProj;
// �׸��� ���
matrix	g_matLightView, g_matLightProj;

////////////////////////////////////////////////////////////////
// Albedo �ؽ���
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

texture g_AnimTexture;
sampler	AnimSampler = sampler_state
{
	texture = g_AnimTexture;

	minfilter = linear;
	magfilter = linear;
};

////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////
// Spec �ؽ���

bool	g_bSpecMap = false;
texture	g_SpecTexture;
sampler	SpecSampler = sampler_state
{
	texture = g_SpecTexture;

	minfilter = linear;
	magfilter = linear;
};

////////////////////////////////////////////////////////////////
// DMesh �����
matrix	g_Bone[50];

//��Ű�� ������ �޴� ���� ����
int		g_iNum;

////////////////////////////////////////////////////////////////

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
// Fog
float	g_fFogStart = 20.f;
float	g_fFogEnd = 30.f;
vector	g_vFogColor = vector(1.f, 1.f, 1.f, 1.f);

///////////////////////////////////////////////////////////////////////////////////////////
// CustomFog

float4	g_CF_vPos;

float	g_CF_Under;
float	g_CF_Over;

float	g_CF_Start;
float4	g_CF_vColor_Start;

float	g_CF_End;
float4	g_CF_vColor_End;

///////////////////////////////////////////////////////////////////////////////////////////
// ����1 / ��� / ����
float	g_fFilter;

//bool	g_bShadow;

//����
float	g_fAddAlpha;
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ��ǻ�� ���� �÷�

struct VS_IN_Color
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT_Color
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
};

VS_OUT_Color		VS_Color(VS_IN_Color In)
{
	VS_OUT_Color Out = (VS_OUT_Color)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

// ��������
struct VS_OUT_Color_Stage
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos	: TEXCOORD1;
	float		fFog		: TEXCOORD2;
	vector		fWorldPos	: TEXCOORD3;
};

VS_OUT_Color_Stage		VS_Color_Stage(VS_IN_Color In)
{
	VS_OUT_Color_Stage Out = (VS_OUT_Color_Stage)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;	

	Out.fFog = saturate(1.f - (g_fFogEnd - mul(In.vPosition, matWV).z) / (g_fFogEnd - g_fFogStart));	
	Out.fWorldPos = mul(In.vPosition, g_matWorld);
	
	return Out;
}

// �ν�ź��
struct VS_IN_Color_Instance
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
	vector		vColor		: COLOR0;
	vector		vRight		: COLOR1;
	vector		vUp			: COLOR2;
	vector      vLook		: COLOR3;
	vector		vPos		: COLOR4;
};

VS_OUT_Color		VS_Color_Instance(VS_IN_Color_Instance In)
{
	VS_OUT_Color Out = (VS_OUT_Color)0;

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
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

// �Ϲ�
struct PS_OUT_Color
{
	vector		vColor		:	COLOR0;
	vector		vEmission	:	COLOR1;
	vector		vNormal		:	COLOR2;
	vector		vDepth		:	COLOR3;
};

PS_OUT_Color		PS_Color(VS_OUT_Color In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);

	////////////////////////////////////////////////////////////////////////////
	// Glow && Mask �ؽ�ó

	// UV�� �ø�
	if (g_fGlow_U_Min <= In.vTexUV.x && g_fGlow_U_Max > In.vTexUV.x &&
		g_fGlow_V_Min <= In.vTexUV.y && g_fGlow_V_Max > In.vTexUV.y)
	{
		// ���̽� + �ؽ�ó �۷ο� (0.f~1.f)
		float3 vGlow = tex2D(MaskSampler, In.vTexUV).rgb;
		float fGlow = saturate(g_fBaseGlow + vGlow.r * g_fTexGlowR + vGlow.g * g_fTexGlowG + vGlow.b * g_fTexGlowB);

		// �۷ο� ����
		Out.vEmission = saturate(Out.vColor) * fGlow * g_fGlowPower * float4(g_fGlowColorR, g_fGlowColorG, g_fGlowColorB, 1.f);
		//Out.vEmission = Out.vColor * fGlow * g_fGlowPower;
	}

	////////////////////////////////////////////////////////////////////////////

	// ī�� ���ÿ� ���
	if (0.5f < g_fFilter) 
	{
		Out.vColor.g += 0.2f;
	}

	return Out;
}

PS_OUT_Color		PS_Color_Stage(VS_OUT_Color_Stage In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 1.f);
	
	///////////////////////////////////////////////////////////////////////
	//// CustomFog	
	float fDis = distance(g_CF_vPos.xyz, In.fWorldPos.xyz);
	float fCustomFog = saturate(1.f - (g_CF_End - fDis) / (g_CF_End - g_CF_Start));
	
	// ������ ���� ������ ������ ����
	float4 vCustomFogColor = (g_CF_vColor_Start * (1.f - fCustomFog)) + (g_CF_vColor_End * fCustomFog);

	// ���� ���� ���� �����Ǵ� ������ ����
	if (fDis <= g_CF_Start)
	{
		vCustomFogColor.a *= g_CF_Under;
	}
	else if (fDis >= g_CF_End)
	{
		vCustomFogColor.a *= g_CF_Over;
	}

	///////////////////////////////////////////////////////////////////////
	// ����
	float4 vFogColor = g_vFogColor * saturate(In.fFog);
	///////////////////////////////////////////////////////////////////////

	float fRatio = vCustomFogColor.a / (vCustomFogColor.a + vFogColor.a + 0.01f);
	fRatio = saturate(fRatio);

	Out.vEmission.rgb = (vCustomFogColor.rgb * fRatio) + (vFogColor.rgb * (1.f - fRatio));
	Out.vEmission.a = saturate(1.f - (1.f - vCustomFogColor.a) * (1.f - vFogColor.a));

	Out.vColor.rgb *= saturate(1.f - Out.vEmission.a);
	
	////////////////////////////////////////////////////////////////////////////
	// Glow && Mask �ؽ�ó

	// UV�� �ø�
	if (g_fGlow_U_Min <= In.vTexUV.x && g_fGlow_U_Max > In.vTexUV.x &&
		g_fGlow_V_Min <= In.vTexUV.y && g_fGlow_V_Max > In.vTexUV.y)
	{
		// ���̽� + �ؽ�ó �۷ο� (0.f~1.f)
		float3 vGlow = tex2D(MaskSampler, In.vTexUV).rgb;
		float fGlow = saturate(g_fBaseGlow + vGlow.r * g_fTexGlowR + vGlow.g * g_fTexGlowG + vGlow.b * g_fTexGlowB);

		// �۷ο� ����
		Out.vEmission += Out.vColor * fGlow * g_fGlowPower * float4(g_fGlowColorR, g_fGlowColorG, g_fGlowColorB, 1.f);
		//Out.vEmission = Out.vColor * fGlow * g_fGlowPower;
	}

	////////////////////////////////////////////////////////////////////////////

	return Out;
}

PS_OUT_Color		PS_Color_Emission(VS_OUT_Color In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;

	Out.vEmission = tex2D(BaseSampler, In.vTexUV);
	Out.vEmission.a = 1.f;

	return Out;
}

// Alpha
struct PS_OUT_Color_Alpha
{
	vector		vColor		:	COLOR0;
	vector		vBlur		:	COLOR1;
};

PS_OUT_Color_Alpha		PS_Color_Alpha(VS_OUT_Color In)
{
	PS_OUT_Color_Alpha Out = (PS_OUT_Color_Alpha)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.w += g_fAddAlpha;

	Out.vBlur = vector(0.f, 0.f, 0.f, 0.f);

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ������ ����

// ���ؽ� �� �ƿ�
struct VS_IN_Shadow
{
	vector		vPosition	: POSITION;
};

struct VS_OUT_Shadow
{
	vector		vPosition	: POSITION;
	vector		vProjPos	: TEXCOORD0;
};

// ���ؽ� ���̴�
VS_OUT_Shadow		VS_Shadow(VS_IN_Shadow In)
{
	VS_OUT_Shadow	Out = (VS_OUT_Shadow)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matLightView);
	matWVP = mul(matWV, g_matLightProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

// �ȼ� �ƿ�
struct PS_OUT_Shadow
{
	// �׸��� mrt
	vector		vLightDepth	:	COLOR0;
};

// �ȼ� ���̴�
PS_OUT_Shadow		PS_Shadow(VS_OUT_Shadow In)
{
	PS_OUT_Shadow	Out = (PS_OUT_Shadow)0;

	Out.vLightDepth = vector (0.f, 0.f, 0.f, 1.f);

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ��ǻ�� ���� �÷�_DMesh

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

	int iNum = g_iNum-1;
	for (int i = 0; i < iNum; ++i)
	{
		fLast -= In.vBlendWeights[i];
		vPos += mul(In.vPosition, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
		vNormal += mul(In.vNormal, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
	}
	vPos += fLast * mul(In.vPosition, g_Bone[int(In.vBlendIndices[iNum])]);
	vPos.w = 1.f;
	vNormal += fLast*mul(In.vNormal, g_Bone[int(In.vBlendIndices[iNum])]);
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

struct VS_IN_Color_DInstance
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;

	float4		vBlendWeights	: BLENDWEIGHT;
	float4		vBlendIndices	: BLENDINDICES;

	vector		vColor		: COLOR0;
	vector		vRight		: COLOR1;
	vector		vUp			: COLOR2;
	vector      vLook		: COLOR3;
	vector		vPos		: COLOR4;
};

VS_OUT_Color_DMesh		VS_Color_DInstance(VS_IN_Color_DInstance In)
{
	VS_OUT_Color_DMesh Out = (VS_OUT_Color_DMesh)0;

	int iAnimIndex = (int)In.vPos.w;
	int iStartIndex = iAnimIndex * 200;

	float2 vUV;

	
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
	matWorld._44 = 0.f;

	matrix matWV, matWVP;
	matWV = mul(matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	/*Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;*/

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	

	///////////////////////////////////////////////////////////////////
	//�ϵ���� ��Ű��

	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);
	float4 vNormal = float4(0.f, 0.f, 0.f, 0.f);
	float fLast = 1.f;

	int4 iIndexArray = D3DCOLORtoUBYTE4(In.vBlendIndices);

	int iNum = g_iNum - 1;
	matrix matAnim;
	for (int i = 0; i < iNum; ++i)
	{
		int	   iIndex = iStartIndex;
		iIndex += int(In.vBlendIndices[i]) * 4;

		vUV.x = float(iIndex % 256) / 256;
		vUV.y = float(iIndex / 256) / 256;
		float4 mat1 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
		++iIndex;
		vUV.x = float(iIndex % 256) / 256;
		vUV.y = float(iIndex / 256) / 256;
		float4 mat2 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
		++iIndex;
		vUV.x = float(iIndex % 256) / 256;
		vUV.y = float(iIndex / 256) / 256;
		float4 mat3 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
		++iIndex;
		vUV.x = float(iIndex % 256) / 256;
		vUV.y = float(iIndex / 256) / 256;
		float4 mat4 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
	
		matAnim._11 = mat1.x;
		matAnim._12 = mat1.y;
		matAnim._13 = mat1.z;
		matAnim._14 = mat1.w;

		matAnim._21 = mat2.x;
		matAnim._22 = mat2.y;
		matAnim._23 = mat2.z;
		matAnim._24 = mat2.w;

		matAnim._31 = mat3.x;
		matAnim._32 = mat3.y;
		matAnim._33 = mat3.z;
		matAnim._34 = mat3.w;

		matAnim._41 = mat4.x;
		matAnim._42 = mat4.y;
		matAnim._43 = mat4.z;
		matAnim._44 = mat4.w;

		fLast -= In.vBlendWeights[i];
		vPos += mul(In.vPosition, matAnim * In.vBlendWeights[i]);
		vNormal += mul(In.vNormal, matAnim * In.vBlendWeights[i]);
	}
	int	   iIndex = iStartIndex;
	iIndex += int(In.vBlendIndices[iNum]) * 4;

	vUV.x = float(iIndex % 256) / 256;
	vUV.y = float(iIndex / 256) / 256;
	float4 mat1 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
	++iIndex;
	vUV.x = float(iIndex % 256) / 256;
	vUV.y = float(iIndex / 256) / 256;
	float4 mat2 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
	++iIndex;
	vUV.x = float(iIndex % 256) / 256;
	vUV.y = float(iIndex / 256) / 256;
	float4 mat3 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));
	++iIndex;
	vUV.x = float(iIndex % 256) / 256;
	vUV.y = float(iIndex / 256) / 256;
	float4 mat4 = tex2Dlod(AnimSampler, float4(vUV.x, vUV.y, 0, 0));

	matAnim._11 = mat1.x;
	matAnim._12 = mat1.y;
	matAnim._13 = mat1.z;
	matAnim._14 = mat1.w;

	matAnim._21 = mat2.x;
	matAnim._22 = mat2.y;
	matAnim._23 = mat2.z;
	matAnim._24 = mat2.w;

	matAnim._31 = mat3.x;
	matAnim._32 = mat3.y;
	matAnim._33 = mat3.z;
	matAnim._34 = mat3.w;

	matAnim._41 = mat4.x;
	matAnim._42 = mat4.y;
	matAnim._43 = mat4.z;
	matAnim._44 = mat4.w;

	vPos += fLast * mul(In.vPosition, matAnim);
	vPos.w = 1.f;
	vNormal += fLast * mul(In.vNormal, matAnim);
	vNormal.w = 0.f;

	///////////////////////////////////////////////////////////////////

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

	////////////////////////////////////////////////////////////////////////////
	// Mouse �ؽ�ó

	if (g_bMouse && In.vTexUV.x < 0.25f && In.vTexUV.y > 0.75f)
	{
		Out.vColor = tex2D(MouseSampler, float2(In.vTexUV.x + g_fMouseU, In.vTexUV.y + g_fMouseV - 0.75f));
	}

	////////////////////////////////////////////////////////////////////////////
	// Glow && Mask �ؽ�ó

	// UV�� �ø�
	if (g_fGlow_U_Min <= In.vTexUV.x && g_fGlow_U_Max > In.vTexUV.x &&
		g_fGlow_V_Min <= In.vTexUV.y && g_fGlow_V_Max > In.vTexUV.y)
	{
		// ���̽� + �ؽ�ó �۷ο� (0.f~1.f)
		float3 vGlow = tex2D(MaskSampler, In.vTexUV).rgb;
		float fGlow = saturate(g_fBaseGlow + vGlow.r * g_fTexGlowR + vGlow.g * g_fTexGlowG + vGlow.b * g_fTexGlowB);

		// �۷ο� ����
		Out.vEmission = Out.vColor * fGlow * g_fGlowPower * float4(g_fGlowColorR, g_fGlowColorG, g_fGlowColorB, 1.f);
		//Out.vEmission = Out.vColor * fGlow * g_fGlowPower;
	}

	////////////////////////////////////////////////////////////////////////////
	// Spec �ؽ�ó

	if (g_bSpecMap)
	{
		float4 vSpec = tex2D(SpecSampler, In.vTexUV);
	}

	////////////////////////////////////////////////////////////////////////////
	// DMesh Disolve

	float fMul = 2000.f; // ���е�
	float fDiv = 150.f; // ȭ�� ��ü�� �� �丷 �Ұ���
	float fMax = (1.f / fDiv) * fMul;

	float fY = In.vProjPos.y / In.vProjPos.w;
	fY = (fY + 1.f) * 0.5f;

	float fRatio = (float)(		(int)(fY * fMul) % (int)fMax		) / fMax;

	if (fRatio >= (1.f - g_fFilter))
	{
		Out = (PS_OUT_Color_DMesh)0;
	}

	////////////////////////////////////////////////////////////////////////////

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ������ ����_DMesh

// ���ؽ� �� �ƿ�
struct VS_IN_Shadow_DMesh
{
	vector		vPosition	: POSITION;

	float4		vBlendWeights	: BLENDWEIGHT;
	float4		vBlendIndices	: BLENDINDICES;
};

// VS_OUT_Shadow_DMesh
// VS_OUT_Shadow �� ��ġ

// ���ؽ� ���̴�
VS_OUT_Shadow		VS_Shadow_DMesh(VS_IN_Shadow_DMesh In)
{
	VS_OUT_Shadow	Out = (VS_OUT_Shadow)0;

	///////////////////////////////////////////////////////////////////
	//�ϵ���� ��Ű��

	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);
	float fLast = 1.f;

	int4 iIndexArray = D3DCOLORtoUBYTE4(In.vBlendIndices);

	int iNum = g_iNum-1;
	for (int i = 0; i < iNum; ++i)
	{
		fLast -= In.vBlendWeights[i];
		vPos += mul(In.vPosition, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
	}
	vPos += fLast * mul(In.vPosition, g_Bone[int(In.vBlendIndices[iNum])]);
	vPos.w = 1.f;

	///////////////////////////////////////////////////////////////////

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matLightView);
	matWVP = mul(matWV, g_matLightProj);

	Out.vPosition = mul(vPos, matWVP);
	Out.vProjPos = Out.vPosition;

	return Out;
}

// PS_OUT_Shadow_DMesh
// PS_OUT_Shadow �� ��ġPS_OUT_Shadow

PS_OUT_Shadow		PS_Shadow_DMesh(VS_OUT_Shadow In)
{
	PS_OUT_Shadow	Out = (PS_OUT_Shadow)0;

	Out.vLightDepth = vector (g_fFilter, g_fFilter, g_fFilter, 1.f - g_fFilter);
		
	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ��������Ƽ


struct PS_OUT_Priority
{
	vector		vColor		:	COLOR0;
};

PS_OUT_Priority		PS_Priority(VS_OUT_Color In)
{
	PS_OUT_Priority Out = (PS_OUT_Priority)0;

	//Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor = vector(tex2D(BaseSampler, In.vTexUV).rgb,1.f);

	return Out;
}

struct PS_OUT_RunMap
{
	vector		vColor		:	COLOR0;
	vector		vEmission	:	COLOR1;
	vector		vNormal		:	COLOR2;
	vector		vDepth		:	COLOR3;
};

PS_OUT_RunMap		PS_RunMap(VS_OUT_Color In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;
	
	Out.vColor = tex2D(BaseSampler, In.vTexUV * 5.f); 
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 1.f);

	////////////////////////////////////////////////////////////////////////////
	// Glow && Mask �ؽ�ó

	// UV�� �ø�
	if (g_fGlow_U_Min <= In.vTexUV.x && g_fGlow_U_Max > In.vTexUV.x &&
		g_fGlow_V_Min <= In.vTexUV.y && g_fGlow_V_Max > In.vTexUV.y)
	{
		// ���̽� + �ؽ�ó �۷ο� (0.f~1.f)
		float3 vGlow = tex2D(MaskSampler, In.vTexUV).rgb;
		float fGlow = saturate(g_fBaseGlow + vGlow.r * g_fTexGlowR + vGlow.g * g_fTexGlowG + vGlow.b * g_fTexGlowB);

		// �۷ο� ����
		Out.vEmission = Out.vColor * fGlow * g_fGlowPower * float4(g_fGlowColorR, g_fGlowColorG, g_fGlowColorB, 1.f);
		//Out.vEmission = Out.vColor * fGlow * g_fGlowPower;
	}

	////////////////////////////////////////////////////////////////////////////

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// �н�

technique Default_Device
{
	///////////////////////////////////////////////////////////////////////////////////////
	// ���̽�
	pass Color  
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;
		
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color();
	}
	pass Shadow
	{
		ZEnable = False;
		ZWriteEnable = False;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Shadow();
		pixelshader = compile ps_3_0 PS_Shadow();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	// Stage
	pass Color
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		
		AlphaTestEnable = false;

		//AlphaBlendEnable = false;
		//
		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 1;
		//
		//FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color_Stage();
		pixelshader = compile ps_3_0 PS_Color_Stage();
	}
	pass Shadow
	{
		ZEnable = False;
		ZWriteEnable = False;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Shadow();
		pixelshader = compile ps_3_0 PS_Shadow();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	// Stage_Instance (Ÿ�Ͽ�)

	pass Color_Instance
	{
		ZEnable = True;
		ZWriteEnable = True;

		// ĳ������ �Զ�����... 

		AlphaTestEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color_Instance();
		pixelshader = compile ps_3_0 PS_Color_Stage();
	}
	pass Shadow_Instance
	{
		ZEnable = False;
		ZWriteEnable = False;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color_Instance();
		pixelshader = compile ps_3_0 PS_Shadow();
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// DMesh
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
	pass Shadow_DMesh
	{
		ZEnable = False;
		ZWriteEnable = False;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Shadow_DMesh();
		pixelshader = compile ps_3_0 PS_Shadow_DMesh();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	pass Color_DMesh_Instance
	{
		ZEnable = True;
		ZWriteEnable = True;

		// ĳ������ �Զ�����... 
		AlphaBlendEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color_DInstance();
		pixelshader = compile ps_3_0 PS_Color_DMesh();
	}
	pass Shadow_DMesh_Instance
	{
		ZEnable = False;
		ZWriteEnable = False;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Shadow_DMesh();
		pixelshader = compile ps_3_0 PS_Shadow_DMesh();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	// Debug
	pass Debug
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		
		AlphaTestEnable = false;

		//AlphaTestEnable = false;
		//
		//AlphaTestEnable = true;
		//AlphaFunc = Greater;
		//AlphaRef = 1;
		
		FillMode = WireFrame;
		
		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color();
	}
	// Priority (Debug�� �Բ� ����.)
	pass Priority // �Ӹ� ä���� ����� �н�
	{
		ZEnable = false;
		ZWriteEnable = false;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		FillMode = Solid;
				
		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Priority();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	pass RunMap
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaTestEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_RunMap();
	}
	//�Ӹ���_ä���
	pass RunMap_temp
	{
		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_RunMap();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	// ����
	pass Color_Alpha
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color_Alpha();
	}
	//�Ӹ���_ä���
	pass Alpha_temp
	{
		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color_Alpha();
	}
	///////////////////////////////////////////////////////////////////////////////////////
};