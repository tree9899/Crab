#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class Collider;
class Navigation;
NS_END

NS_BEGIN(Client)

class Player final : public ContainerObject
{
public:
	enum {
		IDLE		= 0x00000001, 
		RUN			= 0x00000002,
		ATTACK		= 0x00000004,
	};
private:
	Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Player(const Player& Prototype); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;  /* 원형객체가 만들어질 때 호출되는 함수. 무거운 초기화작업.  */
	virtual HRESULT Initialize(void* pArg) override;   /* 원형을 복제하여 만든 사본객체가 생성시에 호출되는 함수. 추가적인 초기화를 위해 */
	virtual void Update_Priority(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Update_Late(_float fTimeDelta) override;
	virtual HRESULT Render() override;	

private:
	_uint				m_iState = {};
	Navigation*		m_pNavigationCom = { nullptr };
	Collider* m_pColliderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

public:
	static Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


NS_END