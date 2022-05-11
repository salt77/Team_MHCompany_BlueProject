

// 기본 행렬
matrix	g_matWorld, g_matView, g_matProj;
// 그림자 계산
matrix	g_matLightView, g_matLightProj;

// Albedo 텍스쳐
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

// DMesh 본행렬
matrix	g_Bone[50];

// DMesh 캐릭터 입
bool	g_bMouse;
float	g_fMouseU;
float	g_fMouseV;

texture	g_MouseTexture;
sampler	MouseSampler = sampler_state
{
	texture = g_MouseTexture;

	minfilter = linear;
	magfilter = linear;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 디퓨즈 랜더 컬러

struct VS_IN_Color
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
};
struct VS_OUT_Color
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
};

VS_OUT_Color		VS_Color(VS_IN_Color In)
{
	VS_OUT_Color Out = (VS_OUT_Color)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_OUT_Color
{
	vector		vColor		:	COLOR0;
	vector		vEmission	:	COLOR1;
};

PS_OUT_Color		PS_Color(VS_OUT_Color In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

PS_OUT_Color		PS_Color_Stage(VS_OUT_Color In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a = 1.f;

	return Out;
}

PS_OUT_Color		PS_Color_Emission(VS_OUT_Color In)
{
	PS_OUT_Color Out = (PS_OUT_Color)0;

	Out.vEmission = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 디퓨즈 랜더 맵

struct VS_IN_Map
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
};

struct VS_OUT_Map
{
	vector		vPosition	: POSITION;
	vector		vNormal		: NORMAL;
	vector		vProjPos	: TEXCOORD0;
};

VS_OUT_Map		VS_Map(VS_IN_Map In)
{
	VS_OUT_Map Out = (VS_OUT_Map)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vProjPos = Out.vPosition;
	
	return Out;
}

struct PS_OUT_Map
{
	vector		vNormal		:	COLOR0;
	vector		vDepth		:	COLOR1;
};

PS_OUT_Map		PS_Map(VS_OUT_Map In)
{
	PS_OUT_Map Out = (PS_OUT_Map)0;
	
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 쉐도우 맵핑

// 버텍스 인 아웃
struct VS_IN_Shadow
{
	vector		vPosition	: POSITION;
};

struct VS_OUT_Shadow
{
	vector		vPosition	: POSITION;
	vector		vProjPos	: TEXCOORD0;
};

// 버텍스 쉐이더
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

// 픽셀 아웃
struct PS_OUT_Shadow
{
	// 그림자 mrt
	vector		vLightDepth	:	COLOR0;
};

// 픽셀 쉐이더
PS_OUT_Shadow		PS_Shadow(VS_OUT_Shadow In)
{
	PS_OUT_Shadow	Out = (PS_OUT_Shadow)0;
	Out.vLightDepth.r = In.vProjPos.w * 0.001f;
	Out.vLightDepth.a = 1.f;


	return Out;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 디퓨즈 랜더 컬러_DMesh

struct VS_IN_Color_DMesh
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;

	float4		vBlendWeights	: BLENDWEIGHT;
	float4		vBlendIndices	: BLENDINDICES;
};
struct VS_OUT_Color_DMesh
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;
};

VS_OUT_Color_DMesh		VS_Color_DMesh(VS_IN_Color_DMesh In)
{
	VS_OUT_Color_DMesh Out = (VS_OUT_Color_DMesh)0;
	
	///////////////////////////////////////////////////////////////////
	//하드웨어 스키닝
	
	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);
	float fLast = 1.f;

	int4 iIndexArray = D3DCOLORtoUBYTE4(In.vBlendIndices);

	int iNum = 2;
	for (int i = 0; i < iNum; ++i)
	{
		fLast -= In.vBlendWeights[i];
		vPos += mul(In.vPosition, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
	}
	vPos += fLast * mul(In.vPosition, g_Bone[int(In.vBlendIndices[iNum])]);
	vPos.w = 1.f;

	///////////////////////////////////////////////////////////////////

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vPos, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_OUT_Color_DMesh
{
	vector		vColor		:	COLOR0;
};

PS_OUT_Color_DMesh		PS_Color_DMesh(VS_OUT_Color_DMesh In)
{
	PS_OUT_Color_DMesh Out = (PS_OUT_Color_DMesh)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	
	//입에 대한 계산
	if (g_bMouse && In.vTexUV.x < 0.25f && In.vTexUV.y > 0.75f)
	{
		Out.vColor = tex2D(MouseSampler, float2(In.vTexUV.x + g_fMouseU, In.vTexUV.y + g_fMouseV - 0.75f));
	}
	
	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 디퓨즈 랜더 맵_DMesh

struct VS_IN_Map_DMesh
{
	vector		vPosition		: POSITION;
	vector		vNormal			: NORMAL;

	float4		vBlendWeights	: BLENDWEIGHT;
	float4		vBlendIndices	: BLENDINDICES;
};

struct VS_OUT_Map_DMesh
{
	vector		vPosition		: POSITION;
	vector		vNormal			: NORMAL;
	vector		vProjPos		: TEXCOORD0;
};

VS_OUT_Map_DMesh		VS_Map_DMesh(VS_IN_Map_DMesh In)
{
	VS_OUT_Map_DMesh Out = (VS_OUT_Map_DMesh)0;

	///////////////////////////////////////////////////////////////////
	//하드웨어 스키닝

	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);
	float4 vNormal = float4(0.f, 0.f, 0.f, 0.f);
	float fLast = 1.f;

	int4 iIndexArray = D3DCOLORtoUBYTE4(In.vBlendIndices);

	int iNum = 2;
	for (int i = 0; i < iNum; ++i)
	{
		fLast	-= In.vBlendWeights[i];
		vPos	+= mul(In.vPosition, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
		vNormal	+= mul(In.vNormal, g_Bone[int(In.vBlendIndices[i])]) * In.vBlendWeights[i];
	}
	vPos += fLast * mul(In.vPosition, g_Bone[int(In.vBlendIndices[iNum])]);
	vPos.w = 1.f;
	vNormal += fLast*mul(In.vNormal, g_Bone[int(In.vBlendIndices[iNum])]);
	vNormal.w = 0.f;

	///////////////////////////////////////////////////////////////////

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition	= mul(vPos, matWVP);
	Out.vNormal		= normalize(mul(vNormal, g_matWorld));
	Out.vProjPos	= Out.vPosition;

	return Out;
}

struct PS_OUT_Map_DMesh
{
	vector		vNormal		:	COLOR0;
	vector		vDepth		:	COLOR1;
	//vector		vMask		:	COLOR2;
};

PS_OUT_Map_DMesh		PS_Map_DMesh(VS_OUT_Map_DMesh In)
{
	PS_OUT_Map_DMesh Out = (PS_OUT_Map_DMesh)0;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 1.f, 1.f);	
	//Out.vMask = vector(0.f, 0.f, 1.f, 1.f);

	return Out;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 쉐도우 맵핑_DMesh

// 버텍스 인 아웃
struct VS_IN_Shadow_DMesh
{
	vector		vPosition	: POSITION;

	float4		vBlendWeights	: BLENDWEIGHT;
	float4		vBlendIndices	: BLENDINDICES;
};

// VS_OUT_Shadow_DMesh
// VS_OUT_Shadow 와 일치

// 버텍스 쉐이더
VS_OUT_Shadow		VS_Shadow_DMesh(VS_IN_Shadow_DMesh In)
{
	VS_OUT_Shadow	Out = (VS_OUT_Shadow)0;

	///////////////////////////////////////////////////////////////////
	//하드웨어 스키닝

	float4 vPos = float4(0.f, 0.f, 0.f, 1.f);
	float fLast = 1.f;

	int4 iIndexArray = D3DCOLORtoUBYTE4(In.vBlendIndices);

	int iNum = 2;
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
// PS_OUT_Shadow 와 일치
//
// PS_Shadow_DMesh
// PS_Shadow 와 일치
// 픽셀 아웃

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 패스

technique Default_Device
{
	///////////////////////////////////////////////////////////////////////////////////////
	// 베이스
	pass Color
	{
		ZEnable = True;
		ZWriteEnable = True;
		
		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		
		AlphaTestEnable = false;
		
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color();
	}
	pass Map
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Map();
		pixelshader = compile ps_3_0 PS_Map();
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

		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		
		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color_Stage();
	}
	pass Map
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Map();
		pixelshader = compile ps_3_0 PS_Map();
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
	// DMesh
	pass Color_DMesh
	{
		ZEnable = True;
		ZWriteEnable = True;

		// 캐릭터의 입때문에... 

		AlphaTestEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Color_DMesh();
		pixelshader = compile ps_3_0 PS_Color_DMesh();
	}
	pass Map_DMesh
	{
		ZEnable = True;
		ZWriteEnable = True;

		AlphaBlendEnable = false;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 1;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Map_DMesh();
		pixelshader = compile ps_3_0 PS_Map_DMesh();
	}
	pass Shadow_DMesh
	{
		ZEnable = False;
		ZWriteEnable = False;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		FillMode = Solid;

		vertexshader = compile vs_3_0 VS_Shadow_DMesh();
		pixelshader = compile ps_3_0 PS_Shadow();
	}
	///////////////////////////////////////////////////////////////////////////////////////
	// Debug
	pass Debug
	{
		ZEnable = True;
		ZWriteEnable = False;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;

		FillMode = WireFrame;

		vertexshader = compile vs_3_0 VS_Color();
		pixelshader = compile ps_3_0 PS_Color_Emission();
	}
};