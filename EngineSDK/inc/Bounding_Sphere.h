#pragma once

#include "Bounding.h"

NS_BEGIN(Engine)

class Bounding_Sphere final : public Bounding
{
public:
	typedef struct tagBoundingSphereDesc : public Bounding::BOUNDING_DESC
	{
		_float			fRadius;
	}BOUNDING_SPHERE_DESC;
private:
	Bounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Bounding_Sphere() = default;
public:
	const BoundingSphere* Get_Desc() const {
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
	BoundingSphere*			m_pOriginalDesc = { nullptr };
	BoundingSphere*			m_pDesc = { nullptr };

public:
	static Bounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUNDING_DESC* pInitialDesc);
	virtual void Free() override;
};

NS_END