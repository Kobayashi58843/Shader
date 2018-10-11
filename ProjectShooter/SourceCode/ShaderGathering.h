#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#include "ShaderEnumeration.h"
#include "ShaderStructure.h"

class ShaderGathering
{
public:
	static ShaderGathering* GetInstance()
	{
		static ShaderGathering s_Instance;
		return &s_Instance;
	}
	~ShaderGathering();

	//�w�肵���V�F�[�_�̃|�C���^���������\���̂�Ԃ�.
	ShaderPointer GetStaticMeshShader(const enStaticModelShader Index) const
	{
		return m_StaticMeshShader[Index];
	}

	//�w�肵���R���X�^���g�o�b�t�@�̃|�C���^��Ԃ�.
	ID3D11Buffer* GetStaticCBuffer(const enStaticModelCBuffer Index) const
	{
		return m_pStaticCBuffers[Index];
	}

	//�V�F�[�_�̏����ݒ�.
	void InitShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

private:
	ShaderGathering();
	ShaderGathering(const ShaderGathering& rhs);
	ShaderGathering& operator = (const ShaderGathering& rhs);

	//Dx11.
	ID3D11Device*			m_pDevice11;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pDeviceContext11;	//�f�o�C�X�R���e�L�X�g.

	//�X�^�e�B�b�N���b�V���̃V�F�[�_.
	ShaderPointer m_StaticMeshShader[enStaticModelShader_Max];

	//�R���X�^���g�o�b�t�@(�X�^�e�B�b�N���b�V��).
	ID3D11Buffer* m_pStaticCBuffers[enStaticModelCBuffer_Max];

	//�X�^�e�B�b�N���b�V���p�̃V�F�[�_���쐬.
	void CreateStaticShader();

	//�V�F�[�_�쐬(�X�^�e�B�b�N���b�V��).
	void MakeStaticMeshShader(char* ShaderName, ShaderEntryPoint EntryPoint, ShaderPointer& ShaderPointer);

	//�R���X�^���g�o�b�t�@�쐬.
	void MakeConstantBuffer(ID3D11Buffer* &pConstantBuffer, UINT CBufferSize);
};