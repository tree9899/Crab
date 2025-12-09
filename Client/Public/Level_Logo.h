#pragma once
#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)
class Level_Logo final : public Level
{
private:
	Level_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);

public:
	static Level_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};
NS_END