 #pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL Model final : public Component
{
private:
	Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Model(const Model& Prototype);
	virtual ~Model() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_int Get_BoneIndex(const _char* pBoneName) const;
	_bool is_AnimFinished() const {
		return m_isAnimFinished;
	}

	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;

public:
	void Set_Animation(_uint iAnimationIndex, _bool isLoop = true);

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Render(_uint iMeshIndex);

public:
	HRESULT Bind_Material(class Shader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType, _uint iTextureIndex = 0);
	HRESULT Bind_Bones(class Shader* pShader, const _char* pConstantName, _uint iMeshIndex);
	void Play_Animation(_float fTimeDelta);

private:
	Assimp::Importer		m_Importer = {};
	const aiScene*			m_pAIScene = { nullptr };

private:
	MODEL							m_eModelType = {};
	_uint							m_iNumMeshes = {};
	vector<class Mesh*>			m_Meshes;	
	_float4x4						m_PreTransformMatrix = {};

	_uint							m_iNumMaterials = {};
	vector<class Material*>		m_Materials;

	_uint							m_iNumAnimations = {};
	_uint							m_iCurrentAnimIndex = {};
	vector<class Animation*>		m_Animations;
	_bool							m_isAnimLoop = { false };
	_bool							m_isAnimFinished = { false };

	/* 이 모델에게 영향을 주는 전체뼈들 : 뼈들은 하나하나 독립적이지 않고, 부모 자식관계로 엮여있기때문에.  */
	vector<class Bone*>			m_Bones;	

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(const aiNode* pAINode, _int iParentIndex);
	HRESULT Ready_Animations();

public:
	static Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END
