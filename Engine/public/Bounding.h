#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Bounding abstract : public Base
{
public:
	typedef struct tagBoundingDesc
	{
		_float3		vCenter;
	}BOUNDING_DESC;

protected:
	Bounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~Bounding() = default;


public:
	HRESULT Initialize();
	virtual void Update(_fmatrix WorldMatrix) = 0;
	virtual _bool Intersect(COLLIDER eTargetType, Bounding* pTarget) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl) = 0;
#endif

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };



public:
	virtual void Free() override;
	
};

NS_END