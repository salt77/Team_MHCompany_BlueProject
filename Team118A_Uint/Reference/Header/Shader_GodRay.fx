

// �⺻ ���
matrix	g_matWorld;

vector	g_vLightWorldPos;
matrix	g_matVP;
bool	g_bSun;

// GodRayTexture �ؽ���
texture	g_RayTexture;
sampler	RaySampler = sampler_state
{
	texture = g_RayTexture;

	minfilter = linear;
	magfilter = linear;
};

// DepthMap �ؽ���
texture	g_DepthTexture;
sampler	DepthSampler = sampler_state
{
	texture = g_DepthTexture;

	minfilter = linear;
	magfilter = linear;
};


////////////////////////////////////////////////////////////////////////////////

// �ȼ� �� �ƿ�
struct PS_IN
{
	float2		vTexUV		:	TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor		:	COLOR0;
};

//
float4 RayFilter(float2 UV) 
{
	float4 vRay = tex2D(RaySampler, UV);
	float2 vDepth = tex2D(DepthSampler, UV).rg;

	if (g_bSun && vDepth.r + vDepth.g < 2.f)
	{
		vRay *= 0.f;
	}

	return vRay;
}

// �ȼ� ���̴�
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	
	// ���������� �����Ͽ� ���� ǥ��
	//Out.vColor = RayFilter(In.vTexUV);

	// ���� ȭ�� ��ǥ
	vector vProgPos = mul(g_vLightWorldPos, g_matVP);
	float2 vLightPos = float2(vProgPos.x / vProgPos.w, vProgPos.y / vProgPos.w);
	vLightPos = float2(vLightPos.x * 0.5f + 0.5f, vLightPos.y * -0.5f + 0.5f);
	
	// �ɼ� ��
	float decay = 0.97815;
	float exposure = 0.92;
	float density = 0.966;
	float weight = 0.58767;
	
	// ���� Ƚ��
	int NUM_SAMPLES = 100; 
	
	float2 tc = In.vTexUV;
	float2 deltaTexCoord = (tc - vLightPos);
	
	deltaTexCoord *= 1.0f / NUM_SAMPLES * density;
	float illuminationDecay = 1.0; 
	float4 vColor = RayFilter(tc) * 0.4f;
	
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		tc -= deltaTexCoord;

		float4 vSample = RayFilter(tc) * 0.4f;

		vSample *= illuminationDecay * weight;
		vColor += vSample;

		illuminationDecay *= decay;
	} 
	
	//Out.vColor = float4( vColor.r * exposure,	vColor.g * exposure,	vColor.b * exposure, 1.f);
	Out.vColor = vColor * exposure;

	return Out;
}

// �н�
technique Default_Device
{
	pass Main
	{
		ZEnable = FALSE;
		ZWriteEnable = FALSE;

		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		AlphaTestEnable = false;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};
