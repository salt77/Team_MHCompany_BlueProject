

// �⺻ ���
matrix	g_matWorld;
// GodRay�� ����
vector	g_vColor;

// Albedo �ؽ���
texture	g_BaseTexture;
sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

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
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vTexUV = In.vTexUV;
	
	return Out;
}

// �ȼ� �ƿ�
struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

// �ȼ� ���̴�
PS_OUT		PS_MAIN(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	// �̹����� ������ ������ ������� ����ϰڴ� -> �𼭸��� ������� ���� ����
	//Out.vColor = vector(1.f, 1.f, 1.f, tex2D(BaseSampler, In.vTexUV).a)  * g_vColor;
	Out.vColor = tex2D(BaseSampler, In.vTexUV) * g_vColor;
	
	return Out;
}

// �н�
technique Default_Device
{
	pass LightRay
	{
		ZEnable = TRUE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;
		
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass SunRay
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};
