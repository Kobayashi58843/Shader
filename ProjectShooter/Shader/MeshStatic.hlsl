//グローバル.
Texture2D		g_texColor	: register(t0);
SamplerState	g_samLinear	: register(s0);

/*/========================/ コンスタントバッファ /========================/*/
cbuffer per_mesh : register(b0)
{
	//ワールド.
	matrix g_mW;
	//[ ワールド×ビュー×プロジェクション ] の変換合成行列.
	matrix g_mWVP;
};

cbuffer per_material : register(b1)
{
	//環境光.
	float4 g_Ambient;
	//拡散反射光.
	float4 g_Diffuse;
	//鏡面反射光.
	float4 g_Specular;
};

cbuffer per_frame : register(b2)
{
	//カメラ位置.
	float4 g_vEye		: packoffset(c0);
	//ライト方向.
	float4 g_vLightDir	: packoffset(c1);
	//ライト強度.
	float  g_fIntensity : packoffset(c2);
};
/*/===============================================================/*/

//バーテックスシェーダ出力構造体.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float3 Normal		: TEXCOORD0;
	float2 Tex			: TEXCOORD1;
	float3 Light		: TEXCOORD2;
	float3 EyeVector	: TEXCOORD3;
	float4 PosWorld		: TEXCOORD4;
};

//バーテックスシェーダ.
VS_OUTPUT VS_Main(
	float4 Pos	: POSITION,
	float4 Norm : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//射影変換(ワールド,ビュー,プロジェクション).
	Out.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる : モデルが回転すれば法線も回転させる必要があるため.
	Out.Normal = mul(Norm, (float3x3)g_mW);

	float3 PosWorld = mul(Pos, g_mW);
	Out.PosWorld = mul(Pos, g_mW);

	//視線ベクトル : ワールド空間上での頂点から頂点へ向かうベクトル.
	Out.EyeVector = normalize(g_vEye - PosWorld);

	//テクスチャ座標.
	Out.Tex = Tex;

	return Out;
}

//ピクセルシェーダ.
float4 PS_Main(VS_OUTPUT In) : SV_Target
{
	//テクスチャカラー.
	float4 texColor = g_texColor.Sample(g_samLinear, In.Tex);

	return texColor;
}

/*/================/ テクスチャ無し /================/*/
//バーテックスシェーダ.
VS_OUTPUT VS_NoTex(
float4 Pos	: POSITION,
float4 Norm : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//射影変換(ワールド,ビュー,プロジェクション).
	Out.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる : モデルが回転すれば法線も回転させる必要があるため.
	Out.Normal = mul(Norm, (float3x3)g_mW);

	float3 PosWorld = mul(Pos, g_mW);
	Out.PosWorld = mul(Pos, g_mW);

	//視線ベクトル : ワールド空間上での頂点から頂点へ向かうベクトル.
	Out.EyeVector = normalize(g_vEye - PosWorld);

	return Out;
}

//ピクセルシェーダ.
float4 PS_NoTex(VS_OUTPUT In) : SV_Target
{
	//テクスチャカラー.
	float4 color = float4(0.4f,0.6f,0.8f,1.0f);

	return color;
}
