#pragma once
#pragma warning( disable:4005 )

//�V�F�[�_(�X�^�e�B�b�N���b�V��).
enum enStaticModelShader
{
	enStaticModelShader_NoTex = 0,
	enStaticModelShader_Simple,

	enStaticModelShader_Max,
};

//�R���X�^���g�o�b�t�@(�X�^�e�B�b�N���b�V��).
enum enStaticModelCBuffer
{
	enStaticModelCBuffer_Mesh = 0,
	enStaticModelCBuffer_Material,
	enStaticModelCBuffer_Frame,

	enStaticModelCBuffer_Max,
};