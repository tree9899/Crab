#pragma once

#include "Bounding.h"

NS_BEGIN(Engine)

class Bounding_OBB final : public Bounding
{
public:
	typedef struct tagBoundingOBBDesc : public Bounding::BOUNDING_DESC
	{
		_float3			vExtents;
		_float3			vAngles;
	}BOUNDING_OBB_DESC;

	typedef struct tagOBBDesc 
	{
		_float3			vCenter;
		_float3			vAlignAxis[3];
		_float3			vCenterAxis[3];
	}OBB_DESC;

private:
	Bounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Bounding_OBB() = default;
public:
	const BoundingOrientedBox* Get_Desc() const {
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
	BoundingOrientedBox*			m_pOriginalDesc = { nullptr };
	BoundingOrientedBox*			m_pDesc = { nullptr };

private:
	_bool Intersect_ToOBB(Bounding_OBB* pTarget);
	Bounding_OBB::OBB_DESC Compute_OBBDesc();



public:
	static Bounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUNDING_DESC* pInitialDesc);
	virtual void Free() override;

};

NS_END