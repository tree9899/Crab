#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "DebugDraw.h"

Bounding_AABB::Bounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Bounding { pDevice, pContext }
{
}

HRESULT Bounding_AABB::Initialize(const BOUNDING_DESC* pInitialDesc)
{
    const BOUNDING_AABB_DESC* pDesc = static_cast<const BOUNDING_AABB_DESC*>(pInitialDesc);    

    m_pOriginalDesc = new BoundingBox(pDesc->vCenter, pDesc->vExtents);
    m_pDesc = new BoundingBox(*m_pOriginalDesc);

    return S_OK;
}

void Bounding_AABB::Update(_fmatrix WorldMatrix)
{
    _matrix     TransformMatrix = WorldMatrix;

    TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
    TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
    TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

    m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);
}

_bool Bounding_AABB::Intersect(COLLIDER eTargetType, Bounding* pTarget)
{
    _bool       isColl = { false };

    switch (eTargetType)
    {
    case COLLIDER::AABB:
        /*isColl = m_pDesc->Intersects(*static_cast<Bounding_AABB*>(pTarget)->Get_Desc());*/
        isColl = Intersect_ToAABB(static_cast<Bounding_AABB*>(pTarget));
        break;
    case COLLIDER::OBB:
        isColl = m_pDesc->Intersects(*static_cast<Bounding_OBB*>(pTarget)->Get_Desc());
        break;
    case COLLIDER::SPHERE:
        isColl = m_pDesc->Intersects(*static_cast<Bounding_Sphere*>(pTarget)->Get_Desc());
        break;
    }

    return isColl;
}

HRESULT Bounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl)
{
    DX::Draw(pBatch, *m_pDesc, true == isColl ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

    return S_OK;
}

_bool Bounding_AABB::Intersect_ToAABB(Bounding_AABB* pTarget)
{
    _float3     vSourMin = Compute_Min();
    _float3     vDestMin = pTarget->Compute_Min();

    _float3     vSourMax = Compute_Max();
    _float3     vDestMax = pTarget->Compute_Max();

    if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
        return false;

    if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
        return false;

    if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
        return false;
    return true;
}

_float3 Bounding_AABB::Compute_Min()
{
    return _float3(
        m_pDesc->Center.x - m_pDesc->Extents.x, 
        m_pDesc->Center.y - m_pDesc->Extents.y, 
        m_pDesc->Center.z - m_pDesc->Extents.z);
}

_float3 Bounding_AABB::Compute_Max()
{
    return _float3(
        m_pDesc->Center.x + m_pDesc->Extents.x,
        m_pDesc->Center.y + m_pDesc->Extents.y,
        m_pDesc->Center.z + m_pDesc->Extents.z);
}

Bounding_AABB* Bounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUNDING_DESC* pDesc)
{
    Bounding_AABB* pInstance = new Bounding_AABB(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX("Failed to Created : Bounding_AABB");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void Bounding_AABB::Free()
{
    __super::Free();

    Safe_Delete(m_pOriginalDesc);
    Safe_Delete(m_pDesc);
}
