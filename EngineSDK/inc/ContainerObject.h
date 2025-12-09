#pragma once

#include "GameObject.h"

/* 파트 오브젝트 여러개를 들고 있어주기위한 기능ㅇㅡㄹ 한다. */

NS_BEGIN(Engine)

class ENGINE_DLL ContainerObject abstract : public GameObject
{
protected:
	ContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ContainerObject(const ContainerObject& Prototype);
	virtual ~ContainerObject() = default;

public:
	class PartObject* Find_PartObject(const _wstring& strPartTag);

public:
	virtual HRESULT Initialize_Prototype() override; /* 원형객체가 만들어질 때 호출되는 함수. 무거운 초기화작업.  */
	virtual HRESULT Initialize(void* pArg) override; /* 원형을 복제하여 만든 사본객체가 생성시에 호출되는 함수. 추가적인 초기화를 위해 */
	virtual void Update_Priority(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Update_Late(_float fTimeDelta) override;
	virtual HRESULT Render() override;	

protected:
	map<const _wstring, class PartObject*>		m_PartObjects;

protected:
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartObjTag, void* pArg);


public:	
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END