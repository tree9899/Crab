#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class Shader;
class Model;
class Texture;
NS_END

NS_BEGIN(Client)
class Mesh_Terrain final : public GameObject
{
private:
	Mesh_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Mesh_Terrain(const Mesh_Terrain& prototype);
	virtual ~Mesh_Terrain() = default;

public:
	// 프로토타입용
	virtual HRESULT Initialize_Prototype() override;
	// 실제 인스턴스용
	virtual HRESULT Initialize(void* pArg) override;
	virtual void   Update(_float fTimeDelta) override;
	virtual void   Update_Late(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	Model* m_pModelCom = { nullptr };
	Shader* m_pShaderCom = { nullptr };
	Texture* m_pTextureCom = { nullptr };

public:
	// 프로토타입 생성
	static Mesh_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END