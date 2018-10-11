#pragma once
#pragma warning( disable:4005 )

//シェーダ(スタティックメッシュ).
enum enStaticModelShader
{
	enStaticModelShader_NoTex = 0,
	enStaticModelShader_Simple,

	enStaticModelShader_Max,
};

//コンスタントバッファ(スタティックメッシュ).
enum enStaticModelCBuffer
{
	enStaticModelCBuffer_Mesh = 0,
	enStaticModelCBuffer_Material,
	enStaticModelCBuffer_Frame,

	enStaticModelCBuffer_Max,
};