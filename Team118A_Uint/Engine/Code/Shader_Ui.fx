// �⺻ ���
matrix	g_matWorld, g_matView, g_matProj;

float2	g_vDrawUVMin;
float2	g_vDrawUVMax;
float2	g_vMaskUVMin;
float2	g_vMaskUVMax;
vector	g_vImageColor;

float2	g_vFillCircle;
float	g_fFillRatio;
// Albedo �ؽ���
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

// ParentMask �ؽ���
texture	g_MaskTexture;
sampler MaskSampler = sampler_state
{
	texture = g_MaskTexture;

	minfilter = linear;
	magfilter = linear;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// ��ǻ�� ���� ���̴�

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
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	if (0.5f > In.vTexUV.x)
	{
		Out.vTexUV.x = g_vDrawUVMin.x;
	}
	else
	{
		Out.vTexUV.x = g_vDrawUVMax.x;
	}

	if (0.5f > In.vTexUV.y)
	{
		Out.vTexUV.y = g_vDrawUVMin.y;
	}
	else
	{
		Out.vTexUV.y = g_vDrawUVMax.y;
	}


	return Out;
}

// �ȼ� �ƿ�
struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

// �ȼ� ���̴�
PS_OUT		PS_MAIN_Light(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	if (g_vMaskUVMin.x <= In.vTexUV.x && g_vMaskUVMax.x >= In.vTexUV.x && 
		g_vMaskUVMin.y <= In.vTexUV.y && g_vMaskUVMax.y >= In.vTexUV.y)
	{
		Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vImageColor;
	}

	return Out;
}


struct VS_OUT_SCROLL_CHILD
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector		vMask		: TEXCOORD1;
};

// ��ũ�� �� �ڽ� 
VS_OUT_SCROLL_CHILD		VS_SCROLL_CHILD(VS_IN In)
{
	VS_OUT_SCROLL_CHILD Out = (VS_OUT_SCROLL_CHILD)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	if (0.5f > In.vTexUV.x)
	{
		Out.vTexUV.x = g_vDrawUVMin.x;
	}
	else
	{
		Out.vTexUV.x = g_vDrawUVMax.x;
	}

	if (0.5f > In.vTexUV.y)
	{
		Out.vTexUV.y = g_vDrawUVMin.y;
	}
	else
	{
		Out.vTexUV.y = g_vDrawUVMax.y;
	}
	Out.vMask.x = g_vMaskUVMin.x;
	Out.vMask.y = g_vMaskUVMax.x;

	Out.vMask.z = g_vMaskUVMin.y;
	Out.vMask.w = g_vMaskUVMax.y;
	return Out;
}

// ��ũ�ѹ� �ڽ� Mask
PS_OUT		PS_SCROLL_CHILD(VS_OUT_SCROLL_CHILD In)
{
	PS_OUT Out = (PS_OUT)0;

	if (In.vTexUV.x > In.vMask.x &&
		In.vTexUV.x < In.vMask.y &&
		In.vTexUV.y > In.vMask.z &&
		In.vTexUV.y < In.vMask.w)
	{
		Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vImageColor;
	}

	return Out;
}

// ��ũ�� �� �ڽ� 
VS_OUT_SCROLL_CHILD		VS_MASK(VS_IN In)
{
	VS_OUT_SCROLL_CHILD Out = (VS_OUT_SCROLL_CHILD)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	if (0.5f > In.vTexUV.x)
	{
		Out.vTexUV.x = g_vDrawUVMin.x;
	}
	else
	{
		Out.vTexUV.x = g_vDrawUVMax.x;
	}

	if (0.5f > In.vTexUV.y)
	{
		Out.vTexUV.y = g_vDrawUVMin.y;
	}
	else
	{
		Out.vTexUV.y = g_vDrawUVMax.y;
	}
	Out.vMask.x = g_vMaskUVMin.x;
	Out.vMask.y = g_vMaskUVMax.x;

	Out.vMask.z = g_vMaskUVMin.y;
	Out.vMask.w = g_vMaskUVMax.y;
	return Out;
}

// ��ũ�ѹ� �ڽ� Mask
PS_OUT		PS_MASK(VS_OUT_SCROLL_CHILD In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMaskColor = tex2D(MaskSampler, In.vTexUV);
	vector vBaseColor = tex2D(BaseSampler, In.vTexUV);
	if (In.vTexUV.x > In.vMask.x &&
		In.vTexUV.x < In.vMask.y &&
		In.vTexUV.y > In.vMask.z &&
		In.vTexUV.y < In.vMask.w && 
		vMaskColor.a > 0.f &&
		vBaseColor.a > 0.f)
	{
		Out.vColor = vBaseColor * g_vImageColor;
		Out.vColor.a *= vMaskColor.a;
	}

	return Out;
}


// ���ؽ� ���̴�
VS_OUT		VS_FillCircle(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	if (0.5f > In.vTexUV.x)
	{
		Out.vTexUV.x = g_vDrawUVMin.x;
	}
	else
	{
		Out.vTexUV.x = g_vDrawUVMax.x;
	}

	if (0.5f > In.vTexUV.y)
	{
		Out.vTexUV.y = g_vDrawUVMin.y;
	}
	else
	{
		Out.vTexUV.y = g_vDrawUVMax.y;
	}

	return Out;
}

// �ȼ� ���̴�
PS_OUT		PS_FillCircle(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 vUp = float2(0.f, 1.f);

	// UV 0 ~ 1�� -1 ~ 1�� ����
	float2 vArrangedUv = In.vTexUV * 2.f - 1.f;
	vArrangedUv.y *= -1.f;

	// ���� UV�� vUp�� ���� ��
	float fAngleDraw	= dot(normalize(vUp), normalize(vArrangedUv));

	// �׷��ߵ� ���� ������
	float fAngleNot		= dot(normalize(vUp), normalize(g_vFillCircle));

	float fDegree = acos(fAngleDraw);
	fDegree = degrees(fDegree);

	if (In.vTexUV.x < 0.5f)
	{
		fDegree = 360.f - fDegree;

	}

	if (g_fFillRatio >= fDegree)
	{
	}
	else
	{
		Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vImageColor;
	}

	return Out;
}
// �н�
technique Default_Device
{
	pass Light			//	0
	{
		ZEnable = True;
		ZWriteEnable = false;

		// ���� ���� ����
		AlphaBlendEnable = true;
		SRCBLEND	= SRCALPHA; 
		DESTBLEND	= INVSRCALPHA;
		
		FillMode = Solid;

		CullMode = None;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN_Light();
	}

	pass Mask			//	1
	{
		ZEnable = True;
		ZWriteEnable = false;

		// ���� ���� ����
		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		FillMode = Solid;

		CullMode = None;

		vertexshader = compile vs_3_0 VS_SCROLL_CHILD();
		pixelshader = compile ps_3_0 PS_SCROLL_CHILD();
	}

	pass ParentMask			//	2
	{
		ZEnable = True;
		ZWriteEnable = false;

		// ���� ���� ����
		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		FillMode = Solid;

		CullMode = None;

		vertexshader = compile vs_3_0 VS_MASK();
		pixelshader = compile ps_3_0 PS_MASK();
	}

	pass FillCircle			//	3
	{
		ZEnable = True;
		ZWriteEnable = false;

		// ���� ���� ����
		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		FillMode = Solid;

		CullMode = None;

		vertexshader = compile vs_3_0 VS_FillCircle();
		pixelshader = compile ps_3_0 PS_FillCircle();
	}
};