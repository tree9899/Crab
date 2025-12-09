#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "DebugDraw.h"

Bounding_OBB::Bounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Bounding { pDevice, pContext }
{
}

HRESULT Bounding_OBB::Initialize(const BOUNDING_DESC* pInitialDesc)
{
    const BOUNDING_OBB_DESC* pDesc = static_cast<const BOUNDING_OBB_DESC*>(pInitialDesc);

    _float4     vRotation = {};
    XMStoreFloat4(&vRotation, 
        XMQuaternionRotationRollPitchYaw(XMConvertToRadians(pDesc->vAngles.x), XMConvertToRadians(pDesc->vAngles.y), XMConvertToRadians(pDesc->vAngles.z)));

    m_pOriginalDesc = new BoundingOrientedBox(pDesc->vCenter, pDesc->vExtents, vRotation);
    m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

    return S_OK;
}

void Bounding_OBB::Update(_fmatrix WorldMatrix)
{
    m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool Bounding_OBB::Intersect(COLLIDER eTargetType, Bounding* pTarget)
{
    _bool       isColl = { false };

    switch (eTargetType)
    {
    case COLLIDER::AABB:
        isColl = m_pDesc->Intersects(*static_cast<Bounding_AABB*>(pTarget)->Get_Desc());
        break;
    case COLLIDER::OBB:
        /*isColl = m_pDesc->Intersects(*static_cast<Bounding_OBB*>(pTarget)->Get_Desc());*/
        isColl = Intersect_ToOBB(static_cast<Bounding_OBB*>(pTarget));
        break;
    case COLLIDER::SPHERE:
        isColl = m_pDesc->Intersects(*static_cast<Bounding_Sphere*>(pTarget)->Get_Desc());
        break;
    }

    return isColl;
}

HRESULT Bounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _bool isColl)
{
    DX::Draw(pBatch, *m_pDesc, true == isColl ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

    return S_OK;
}

_bool Bounding_OBB::Intersect_ToOBB(Bounding_OBB* pTarget)
{
    OBB_DESC      OBBDesc[2] = {
        Compute_OBBDesc(),
        pTarget->Compute_OBBDesc()
    };

    _float       fLength[3] = {};

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            fLength[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

            fLength[1] =
                fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
                fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
                fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));


            fLength[2] =
                fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
                fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
                fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]),XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

            if (fLength[0] > fLength[1] + fLength[2])
                return false;
        }

    }
    return true;
}

Bounding_OBB::OBB_DESC Bounding_OBB::Compute_OBBDesc()
{
    OBB_DESC            OBBDesc{};

    OBBDesc.vCenter = m_pDesc->Center;
    
    _float3     vPoints[8] = {};
    m_pDesc->GetCorners(vPoints);

    _vector     vAxis[3];
    vAxis[0] = XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4]);
    vAxis[1] = XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4]);
    vAxis[2] = XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4]);
    
    for (size_t i = 0; i < 3; i++)
    {
        XMStoreFloat3(&OBBDesc.vCenterAxis[i], vAxis[i] * 0.5f);
        XMStoreFloat3(&OBBDesc.vAlignAxis[i], XMVector3Normalize(vAxis[i]));
    }
    return OBBDesc;
}

Bounding_OBB* Bounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const BOUNDING_DESC* pDesc)
{
    Bounding_OBB* pInstance = new Bounding_OBB(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX("Failed to Created : Bounding_OBB");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void Bounding_OBB::Free()
{
    __super::Free();

    Safe_Delete(m_pOriginalDesc);
    Safe_Delete(m_pDesc);
}
