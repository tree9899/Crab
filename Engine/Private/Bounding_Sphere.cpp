#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "DebugDraw.h"

Bounding_Sphere::Bounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Bounding { pDevice, pContext }
{
}

HRESULT Bounding_Sphere::Initialize(const BOUNDING_DESC* pInitialDesc)
{
    const BOUNDING_SPHERE_DESC* pDesc = static_cast<const BOUNDING_SPHERE_DESC*>(pInitialDesc);

    m_pOriginalDesc = new BoundingSphere(pDesc->vCenter, pDesc->fRadius);
    m_pDesc = new BoundingSphere(*m_pOriginalDesc);

    return S_OK;
}

void Bounding_Sphere::Update(_fmatrix WorldMatrix)
{

    m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool Bounding_Sphere::Intersect(COLLIDER eTargetType, Bounding* pTarget)
{
    _bool       isColl = { false };

    switch (eTargetType)
    {
    case COLLIDER::AABB:
        isColl = m_pDesc->Intersects(*static_cast<Bounding_AABB*>(pTarget)->Get_Desc());
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

HRESULT Bounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl)
{
    DX::Draw(pBatch, *m_pDesc, true == isColl ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

    return S_OK;
}

Bounding_Sphere* Bounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUNDING_DESC* pDesc)
{
    Bounding_Sphere* pInstance = new Bounding_Sphere(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX("Failed to Created : Bounding_Sphere");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void Bounding_Sphere::Free()
{
    __super::Free();

    Safe_Delete(m_pOriginalDesc);
    Safe_Delete(m_pDesc);
}
