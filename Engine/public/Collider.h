#pragma once

#include "Component.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

NS_BEGIN(Engine)

class ENGINE_DLL Collider final : public Component
{
private:
	Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Collider(const Collider& Prototype);
	virtual ~Collider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDER eType);
	virtual HRESULT Initialize(void* pArg);
public:
	void Update(_fmatrix WorldMatrix);
	_bool Intersect(Collider* pTargetCollider);

#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif

private:	
	COLLIDER					m_eType = {};
	class Bounding*			m_pBounding = { nullptr };
	_bool						m_isColl = { false };
	
#ifdef _DEBUG
private:
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*									m_pEffect = { nullptr };
	ID3D11InputLayout*								m_pInputLayout = { nullptr };
	
#endif


public:
	static Collider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;


};

NS_END