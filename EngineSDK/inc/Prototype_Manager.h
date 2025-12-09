#pragma once

/* CGameObject, CComponent들의 원형을 보관한다 */
/* 레벨별로 구분하여 보관한다. 따로 GameObject, Component구분짓진 않는다 .*/

#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "Navigation.h"
#include "Transform.h"
#include "Collider.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"

NS_BEGIN(Engine)

class Prototype_Manager final : public Base
{
private:
	Prototype_Manager();
	virtual ~Prototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, Base* pPrototype);
	Base* Clone_Prototype(PROTOTYPE ePrototypeID, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	void Clear(_uint iClearLevelID);

private:
	_uint								m_iNumLevels = {};
	map<const _wstring, Base*>*		m_pPrototypes = nullptr;

private:
	Base* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static Prototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

NS_END