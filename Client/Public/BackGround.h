#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class VIBuffer_Rect;
class Shader;
class Texture;
NS_END

NS_BEGIN(Client)
class BackGround final : public UIObject
{
protected:
	BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BackGround(const BackGround& Prototype); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~BackGround() = default;

public:
	virtual HRESULT Initialize_Prototype(); /* 원형객체가 만들어질 때 호출되는 함수. 무거운 초기화작업.  */
	virtual HRESULT Initialize(void* pArg); /* 원형을 복제하여 만든 사본객체가 생성시에 호출되는 함수. 추가적인 초기화를 위해 */
	virtual void Update_Priority(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Update_Late(_float fTimeDelta);
	virtual HRESULT Render();

private:
	VIBuffer_Rect* m_pVIBufferCom = { nullptr };
	Texture* m_pTextureCom = { nullptr };
	Shader* m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
NS_END