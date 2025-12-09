#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL PartObject abstract : public GameObject
{
public:
	typedef struct tagPartObjectDesc : public GameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentMatrix = { nullptr };
	}PARTOBJECT_DESC;

protected:
	PartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	PartObject(const PartObject& Prototype);
	virtual ~PartObject() = default;
public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체가 만들어질 때 호출되는 함수. 무거운 초기화작업.  */
	virtual HRESULT Initialize(void* pArg) override; /* 원형을 복제하여 만든 사본객체가 생성시에 호출되는 함수. 추가적인 초기화를 위해 */
	virtual void Update_Priority(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Update_Late(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	const _float4x4* m_pParentMatrix = { nullptr };
	_float4x4		 m_CombinedWorldMatrix = {};

protected:
	void SetUp_CombinedWorldMatrix(_fmatrix ParentMatrix);

public:	
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END