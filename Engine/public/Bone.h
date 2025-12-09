#pragma once

#include "Base.h"
/* 뼈 : aiBone, aiNode, aiNodeAnim */

NS_BEGIN(Engine)

class Bone final : public Base
{
private:
	Bone();
	virtual ~Bone() = default;

public:
	_bool Compare_Name(const _char* pName)
	{
		return !strcmp(pName, m_szName);
	}

	_matrix Get_CombinedTransformationMatrix() {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	const _float4x4* Get_CombinedTransformationMatrixPtr() const {
		return &m_CombinedTransformationMatrix;
	}

	void Set_TransformationMatrix(const _float4x4& TransformationMatrix) {
		m_TransformationMatrix = TransformationMatrix;
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<Bone*>& Bones, _fmatrix PreTransformMatrix);

private:
	_char				m_szName[MAX_PATH] = {};
	_float4x4			m_TransformationMatrix = {}; /* 이 뼈만의 상태(크 * 자 * 이) */
	_float4x4			m_CombinedTransformationMatrix = {}; /* 내`TransformationMatrix * 부모`s CombinedTransformationMatrix */
	_int				m_iParentBoneIndex = { -1 };

public:
	static Bone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	Bone* Clone();
	virtual void Free() override;

};

NS_END