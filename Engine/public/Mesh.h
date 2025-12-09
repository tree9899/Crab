#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL Mesh final : public VIBuffer
{
private:
	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Mesh(const Mesh& Prototype);
	virtual ~Mesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType, const aiMesh* pAIMesh, class Model* pModel, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_Bones(class Shader* pShader, const _char* pConstantName, const vector<class Bone*>& Bones);
private:
	_char			m_szName[MAX_PATH] = {};
	_uint			m_iMaterialIndex = {};
	_uint			m_iNumBones = {}; /* 이 메시를 구성하는 정점들에게 영향을 주는 뼈의 갯수 */

	/* 이 메시에 영향을 주는 뼈들의 인덱스를 모아놓는다. */
	/* 뼈들의 인덱스 : 모델객체가 들고 있는 전체 뼈컨테이너의 특정 인덱스 */
	vector<_uint>	m_BoneIndices;
	_float4x4		m_BoneMatrices[g_iMaxNumBones] = {};

	/* 특정 뼈를 정점에게 적용할 때, 추가 보정을 거쳐서 정점에게 상태를 전달할 수 있도록한다.  */
	vector<_float4x4>	m_OffsetMatrices;


private:
	HRESULT Ready_For_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_For_AnimMesh(const aiMesh* pAIMesh, class Model* pModel);


public:
	static Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const aiMesh* pAIMesh, class Model* pModel, _fmatrix PreTransformMatrix);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END