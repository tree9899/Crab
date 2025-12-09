#pragma once

#include "Base.h"

/* 화면에 그려져야할 객체들을 모아놓는다. */
/* 그려야하는 순서대로 그룹지어서 모은다. 모여있는 객체들의 렌더콜 */

NS_BEGIN(Engine)

class Renderer final : public Base
{
private:
	Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Renderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eGroupID, class GameObject* pRenderObject);
	void Draw();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	list<class GameObject*>			m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::END)];

private:
	void Render_Priority();
	void Render_NonBlend();
	void Render_Blend();
	void Render_UI();

public:
	static Renderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END