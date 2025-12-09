#pragma once

#include "Bounding.h"

NS_BEGIN(Engine)

class Bounding_AABB final : public Bounding
{
public:
	typedef struct tagBoundingAABBDesc : public Bounding::BOUNDING_DESC
	{
		_float3			vExtents;
	}BOUNDING_AABB_DESC;
private:
	Bounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Bounding_AABB() = default;

public:
	const BoundingBox* Get_Desc() const {
		return m_pDesc;
	}

public:
	HRESULT Initialize(const BOUNDING_DESC* pInitialDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(COLLIDER eTargetType, Bounding* pTarget) override;
#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl) override;
#endif
private:
	BoundingBox*			m_pOriginalDesc = { nullptr };
	BoundingBox*			m_pDesc = { nullptr };	

private:
	_bool Intersect_ToAABB(Bounding_AABB* pTarget);
	_float3 Compute_Min();
	_float3 Compute_Max();


public:
	static Bounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUNDING_DESC* pInitialDesc);
	virtual void Free() override;

};

NS_END