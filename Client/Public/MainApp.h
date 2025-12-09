#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class GameInstance;
NS_END

NS_BEGIN(Client)

class MainApp final : public Base
{
private:
	MainApp();
	virtual ~MainApp() = default;

public:
	/* 게임 초기화작업 */
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	GameInstance* m_pGameInstance = { nullptr };

private:
	HRESULT Ready_Gara();
	HRESULT Start_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_For_Static();
public:
	static MainApp* Create();
	virtual void Free() override;
};

NS_END

