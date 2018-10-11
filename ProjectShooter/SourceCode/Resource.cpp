#include"Resource.h"

/*/========/ �R���X�g���N�^ /========/*/
Resource::Resource()
{
	m_hWnd = nullptr;
	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	si.hWnd = nullptr;
	si.pDevice = nullptr;
	si.pDeviceContext = nullptr;

	m_ppStaticModels = nullptr;
	m_ppSkinModels = nullptr;
}

/*/========/ �f�X�g���N�^ /========/*/
Resource::~Resource()
{
	for (int i = 0; i < enStaticModel_Max; i++)
	{
		ReleaseStaticModel((enStaticModel)i);
	}
	for (int i = 0; i < enSkinModel_Max; i++)
	{
		ReleaseSkinModel((enSkinModel)i);
	}

	//�|�C���^�̈���������.
	if (m_ppStaticModels)
	{
		delete[] m_ppStaticModels;
		m_ppStaticModels = nullptr;
	}

	if (m_ppSkinModels)
	{
		delete[] m_ppSkinModels;
		m_ppSkinModels = nullptr;
	}

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}

/*/========================================/ �X�^�e�B�b�N���f�� /========================================/*/
HRESULT Resource::InitStaticModel(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice;
	m_pContext11 = pContext;

	//�ÓI���b�V���̃|�C���^�̃|�C���^�̈���m��.
	m_ppStaticModels = new clsDX9Mesh*[enStaticModel_Max];
	for (int iStModelNo = 0; iStModelNo<enStaticModel_Max; iStModelNo++)
	{
		//nullptr�ŏ�����.
		m_ppStaticModels[iStModelNo] = nullptr;
	}

	return S_OK;
}

HRESULT Resource::CreateStaticModel(LPSTR fileName, enStaticModel enModel)
{
	//�񋓑͈͓̂̔���.
	if (!IsRengeStaticModel(enModel))
	{
		return E_FAIL;
	}

	//���Ƀf�[�^�����邩.
	if (m_ppStaticModels[enModel] != nullptr)
	{
		return E_FAIL;
	}

	//���f���Ǎ�.
	m_ppStaticModels[enModel] = new clsDX9Mesh;
	m_ppStaticModels[enModel]->Init(m_hWnd, m_pDevice11, m_pContext11, fileName);

	return S_OK;
}

HRESULT Resource::ReleaseStaticModel(enStaticModel enModel)
{
	//�񋓑͈͓̂̔���.
	if (!IsRengeStaticModel(enModel))
	{
		return E_FAIL;
	}

	//���Ƀf�[�^���Ȃ��Ȃ��Ă邩.
	if (m_ppStaticModels[enModel] == nullptr)
	{
		return E_FAIL;
	}

	//�폜����nullptr�Ŗ��߂�.
	delete m_ppStaticModels[enModel];
	m_ppStaticModels[enModel] = nullptr;

	return S_OK;
}

clsDX9Mesh* Resource::GetStaticModels(enStaticModel enModel)
{
	//�͈͓����`�F�b�N.
	if (!IsRengeStaticModel(enModel))
	{
		return nullptr;
	}

	//���f�������邩�`�F�b�N.
	if (m_ppStaticModels[enModel] == nullptr)
	{
		return nullptr;
	}

	//���f���̃|�C���^��Ԃ�.
	return m_ppStaticModels[enModel];
}

void Resource::CreateStaticModelResouce(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	InitStaticModel(hWnd, pDevice, pContext);
	CreateStaticModel("Data\\Ground\\Stage.X", enStaticModel_Ground);
	CreateStaticModel("Data\\Collision\\Sphere.X", enStaticModel_Shpere);
}

//�͈͓����ǂ����`�F�b�N���� : �͈͓��Ɏ��܂��Ă���true.
bool Resource::IsRengeStaticModel(enStaticModel enModel)
{
	if (0 <= enModel && enModel < enStaticModel_Max)
	{
		return true;
	}
	return false;
}

/*/=========================================/ �X�L�����f�� /=========================================/*/

HRESULT Resource::InitSkinModel(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	si.hWnd = hWnd;
	si.pDevice = pDevice;
	si.pDeviceContext = pContext;

	//�ÓI���b�V���̃|�C���^�̃|�C���^�̈���m��.
	m_ppSkinModels = new clsD3DXSKINMESH*[enSkinModel_Max];
	for (int iStModelNo = 0; iStModelNo<enSkinModel_Max; iStModelNo++)
	{
		//nullptr�ŏ�����.
		m_ppSkinModels[iStModelNo] = nullptr;
	}

	return S_OK;
}

HRESULT Resource::CreateSkinModel(LPSTR fileName, enSkinModel enModel)
{
	//�񋓑͈͓̂̔���.
	if (!IsRengeSkinModel(enModel))
	{
		return E_FAIL;
	}

	//���Ƀf�[�^�����邩.
	if (m_ppSkinModels[enModel] != nullptr)
	{
		return E_FAIL;
	}

	//���f���Ǎ�.
	m_ppSkinModels[enModel] = new clsD3DXSKINMESH;
	m_ppSkinModels[enModel]->Init(&si);
	m_ppSkinModels[enModel]->CreateFromX(fileName);

	return S_OK;
}

HRESULT Resource::ReleaseSkinModel(enSkinModel enModel)
{
	//�񋓑͈͓̂̔���.
	if (!IsRengeSkinModel(enModel))
	{
		return E_FAIL;
	}

	//���Ƀf�[�^���Ȃ��Ȃ��Ă邩.
	if (m_ppSkinModels[enModel] == nullptr)
	{
		return E_FAIL;
	}

	//�폜����nullptr�Ŗ��߂�.
	delete m_ppSkinModels[enModel];
	m_ppSkinModels[enModel] = nullptr;

	return S_OK;
}

clsD3DXSKINMESH* Resource::GetSkinModels(enSkinModel enModel)
{
	//�͈͓����`�F�b�N.
	if (!IsRengeSkinModel(enModel))
	{
		return nullptr;
	}

	//���f�������邩�`�F�b�N.
	if (m_ppSkinModels[enModel] == nullptr)
	{
		return nullptr;
	}

	//���f���̃|�C���^��Ԃ�.
	return m_ppSkinModels[enModel];
}

void Resource::CreateSkinModelResouce(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	InitSkinModel(hWnd, pDevice, pContext);
	CreateSkinModel("Data\\Player\\Bozu.x", enSkinModel_Player);
	CreateSkinModel("Data\\Enemy\\Josin01.x", enSkinModel_Enemy);
}

//�͈͓����ǂ����`�F�b�N���� : �͈͓��Ɏ��܂��Ă���true.
bool Resource::IsRengeSkinModel(enSkinModel enModel)
{
	if (0 <= enModel && enModel < enSkinModel_Max)
	{
		return true;
	}
	return false;
}