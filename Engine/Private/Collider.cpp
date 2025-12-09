#include "Collider.h"

#include "DebugDraw.h"


#include "GameInstance.h"

Collider::Collider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice, pContext }
{

}

Collider::Collider(const Collider& Prototype)
    : Component{ Prototype }
    , m_eType { Prototype.m_eType }
#ifdef _DEBUG
    , m_pBatch{ Prototype.m_pBatch }
    , m_pEffect{ Prototype.m_pEffect }
    , m_pInputLayout{ Prototype.m_pInputLayout }
#endif
{
    Safe_AddRef(m_pInputLayout);
}

HRESULT Collider::Initialize_Prototype(COLLIDER eType)
{
    m_eType = eType;

#ifdef _DEBUG
    m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
    m_pEffect = new BasicEffect(m_pDevice);
    m_pEffect->SetVertexColorEnabled(true);

    const void* pShaderInput = {};
    size_t iShaderInputLength = {};

    m_pEffect->GetVertexShaderBytecode(&pShaderInput, &iShaderInputLength);

    if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
        pShaderInput, iShaderInputLength, &m_pInputLayout)))
        return E_FAIL;
#endif

    return S_OK;
}

HRESULT Collider::Initialize(void* pArg)
{
    Bounding::BOUNDING_DESC* pInitialDesc = static_cast<Bounding::BOUNDING_DESC*>(pArg);

    switch (m_eType)
    {
    case COLLIDER::AABB:
        m_pBounding = Bounding_AABB::Create(m_pDevice, m_pContext, pInitialDesc);
        break;
    case COLLIDER::OBB:
        m_pBounding = Bounding_OBB::Create(m_pDevice, m_pContext, pInitialDesc);
        break;
    case COLLIDER::SPHERE:
        m_pBounding = Bounding_Sphere::Create(m_pDevice, m_pContext, pInitialDesc);
        break;
    }

    return S_OK;
}

void Collider::Update(_fmatrix WorldMatrix)
{
    m_pBounding->Update(WorldMatrix);
}

_bool Collider::Intersect(Collider* pTargetCollider)
{
    m_isColl = m_pBounding->Intersect(pTargetCollider->m_eType, pTargetCollider->m_pBounding);

    return m_isColl;
}

#ifdef _DEBUG

HRESULT Collider::Render()
{
    m_pEffect->SetWorld(XMMatrixIdentity());
    m_pEffect->SetView(XMLoadFloat4x4(m_pGameInstance->Get_Transform(D3DTS::VIEW)));
    m_pEffect->SetProjection(XMLoadFloat4x4(m_pGameInstance->Get_Transform(D3DTS::PROJ)));

    m_pEffect->Apply(m_pContext);
    m_pContext->IASetInputLayout(m_pInputLayout);

    m_pBatch->Begin();

    m_pBounding->Render(m_pBatch, m_isColl);

    m_pBatch->End();

    return S_OK;
}

#endif

Collider* Collider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType)
{
    Collider* pInstance = new Collider(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eType)))
    {
        MSG_BOX("Failed to Created : Collider");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Component* Collider::Clone(void* pArg)
{
    Collider* pInstance = new Collider(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Collider");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void Collider::Free()
{
    __super::Free();

#ifdef _DEBUG
    if (false == m_isClone)
    {
        Safe_Delete(m_pBatch);
        Safe_Delete(m_pEffect);
    }      

    Safe_Release(m_pInputLayout);
#endif

    Safe_Release(m_pBounding);


}
