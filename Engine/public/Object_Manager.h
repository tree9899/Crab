#pragma once

/* 게임 내에서 실제 사용을 위한 객체들을 복제하여 생성한 후, 보관한다. */
/* 개발자가 원하는대로 묶어서(CLayer) 레벨별로([]) 구분하여 보관한다. */
/* 레벨이 이용하는 레이어들을 반복적으로 갱신해 준다.  */
/* 렌더까지는 수행하지 않는다.(CRenderer) */

#include "Base.h"

NS_BEGIN(Engine)

class Object_Manager final : public Base
{
private:
	Object_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Object_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	void Update_Priority(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Update_Late(_float fTimeDelta);
	void Clear(_uint iClearLevelID);

public:
	HRESULT Add_GameObject_To_Layer(_uint iProtoLevelIndex, const _wstring& strPrototypeTag, 
		_uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
		
	class GameObject* Find_GameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iIndex);

private:	
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	class GameInstance* m_pGameInstance = { nullptr };

private:	
	map<const _wstring, class Layer*>* m_pLayers = { nullptr };
	_uint								m_iNumLevels = {};

private:
	Layer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);


public:
	static Object_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels);
	virtual void Free();
};

NS_END