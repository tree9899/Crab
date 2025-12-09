#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class GameInstance;
NS_END

NS_BEGIN(Client)

class Loader final : public Base
{
private:
	Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Loader() = default;

public:
	HRESULT Initialize(LEVEL eLoadingLevelID);
	HRESULT Loading();

	bool isFinished() const
	{
		return m_isFinished; //로딩끝났는지 체크
	}

	void Output()
	{
		SetWindowText(g_hWnd, m_szFPS);
	}


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	GameInstance* m_pGameInstance = { nullptr };
	LEVEL m_eLoadingLevelID = { LEVEL::END };

	HANDLE m_hThread = {};
	CRITICAL_SECTION m_CriticalSection = {};

	_bool m_isFinished = { false };
	_tchar m_szFPS[MAX_PATH] = {};

private:
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
	HRESULT Loading_For_MapTool();

public:
	static Loader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLoadingLevelID);
	virtual void Free() override;
};

NS_END