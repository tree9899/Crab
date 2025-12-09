#pragma once
#include "Client_Defines.h"
#include "Level.h"

class MapTool_Level final : public Level
{
private:
	MapTool_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~MapTool_Level() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	//HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tree(const _wstring& strLayerTag);

public:
	static MapTool_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

