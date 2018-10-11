//�O���[�o��.
Texture2D		g_texColor	: register(t0);
SamplerState	g_samLinear	: register(s0);

/*/========================/ �R���X�^���g�o�b�t�@ /========================/*/
cbuffer per_mesh : register(b0)
{
	//���[���h.
	matrix g_mW;
	//[ ���[���h�~�r���[�~�v���W�F�N�V���� ] �̕ϊ������s��.
	matrix g_mWVP;
};

cbuffer per_material : register(b1)
{
	//����.
	float4 g_Ambient;
	//�g�U���ˌ�.
	float4 g_Diffuse;
	//���ʔ��ˌ�.
	float4 g_Specular;
};

cbuffer per_frame : register(b2)
{
	//�J�����ʒu.
	float4 g_vEye		: packoffset(c0);
	//���C�g����.
	float4 g_vLightDir	: packoffset(c1);
	//���C�g���x.
	float  g_fIntensity : packoffset(c2);
};
/*/===============================================================/*/

//�o�[�e�b�N�X�V�F�[�_�o�͍\����.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float3 Normal		: TEXCOORD0;
	float2 Tex			: TEXCOORD1;
	float3 Light		: TEXCOORD2;
	float3 EyeVector	: TEXCOORD3;
	float4 PosWorld		: TEXCOORD4;
};

//�o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_Main(
	float4 Pos	: POSITION,
	float4 Norm : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//�ˉe�ϊ�(���[���h,�r���[,�v���W�F�N�V����).
	Out.Pos = mul(Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹�� : ���f������]����Ζ@������]������K�v�����邽��.
	Out.Normal = mul(Norm, (float3x3)g_mW);

	float3 PosWorld = mul(Pos, g_mW);
	Out.PosWorld = mul(Pos, g_mW);

	//�����x�N�g�� : ���[���h��ԏ�ł̒��_���璸�_�֌������x�N�g��.
	Out.EyeVector = normalize(g_vEye - PosWorld);

	//�e�N�X�`�����W.
	Out.Tex = Tex;

	return Out;
}

//�s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT In) : SV_Target
{
	//�e�N�X�`���J���[.
	float4 texColor = g_texColor.Sample(g_samLinear, In.Tex);

	return texColor;
}

/*/================/ �e�N�X�`������ /================/*/
//�o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_NoTex(
float4 Pos	: POSITION,
float4 Norm : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//�ˉe�ϊ�(���[���h,�r���[,�v���W�F�N�V����).
	Out.Pos = mul(Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹�� : ���f������]����Ζ@������]������K�v�����邽��.
	Out.Normal = mul(Norm, (float3x3)g_mW);

	float3 PosWorld = mul(Pos, g_mW);
	Out.PosWorld = mul(Pos, g_mW);

	//�����x�N�g�� : ���[���h��ԏ�ł̒��_���璸�_�֌������x�N�g��.
	Out.EyeVector = normalize(g_vEye - PosWorld);

	return Out;
}

//�s�N�Z���V�F�[�_.
float4 PS_NoTex(VS_OUTPUT In) : SV_Target
{
	//�e�N�X�`���J���[.
	float4 color = float4(0.4f,0.6f,0.8f,1.0f);

	return color;
}
