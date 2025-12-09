#include "Player.h"
#include "Body.h"
#include "Weapon.h"

#include "GameInstance.h"

Player::Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: ContainerObject { pDevice, pContext }
{
}

Player::Player(const Player& Prototype)
	: ContainerObject{ Prototype }
{
}

HRESULT Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Player::Initialize(void* pArg)
{
    GameObject::GAMEOBJECT_DESC        Desc{};

    Desc.fSpeedPerSec = 10.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.0f);

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    //m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(m_pNavigationCom->Get_CellPos(), 1.f));
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 200.f, 0.f, 1.f));


	return S_OK;
}

void Player::Update_Priority(_float fTimeDelta)
{
    __super::Update_Priority(fTimeDelta);
}

void Player::Update(_float fTimeDelta)
{
    if (GetKeyState(VK_LEFT) & 0x8000)
    {
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);
    }
    if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
    }
    if (GetKeyState(VK_DOWN) & 0x8000)
    {
        m_pTransformCom->Go_Backward(fTimeDelta);
    }

    if (GetKeyState(VK_UP) & 0x8000)
    {
        //m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

        if(m_iState & IDLE)
            m_iState ^= IDLE;

        m_iState |= RUN;
    }
    else
    {
        if (m_iState & RUN)
            m_iState ^= RUN;

        m_iState |= IDLE;
    }

    //m_pTransformCom->Set_State(STATE::POSITION, 
        //m_pNavigationCom->SetUp_OnNavigation(m_pTransformCom->Get_State(STATE::POSITION)));

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

    __super::Update(fTimeDelta);
}

void Player::Update_Late(_float fTimeDelta)
{
    __super::Update_Late(fTimeDelta);

    m_pGameInstance->Add_RenderObject(RENDERGROUP::NONBLEND, this);
}

HRESULT Player::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
    //m_pNavigationCom->Render();
#endif

	return S_OK;
}

HRESULT Player::Ready_Components()
{
    /* For.Com_Navigation */
    Navigation::NAVIGATION_DESC        NavigationDesc{};
    NavigationDesc.iCurrentCellIndex = 10;    

    //if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Navigation"),
    //    TEXT("Com_Navigation"), reinterpret_cast<Component**>(&m_pNavigationCom), &NavigationDesc)))
    //    return E_FAIL;

    /* For.Com_Collider_AABB */
    Bounding_AABB::BOUNDING_AABB_DESC      ColliderDesc{};
    ColliderDesc.vExtents = _float3(0.4f, 0.6f, 0.4f);
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
    

    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider_AABB"), reinterpret_cast<Component**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;
    

	return S_OK;
}

HRESULT Player::Ready_PartObjects()
{
    Body::BODY_DESC            BodyDesc{};
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    BodyDesc.pParentState = &m_iState;

    if (FAILED(__super::Add_PartObject(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Body_Player"), 
        TEXT("Part_Body"), &BodyDesc)))
        return E_FAIL;

    Body* pPartBody = dynamic_cast<Body*>(Find_PartObject(TEXT("Part_Body")));
    if (nullptr == pPartBody)
        return E_FAIL;


    Weapon::WEAPON_DESC            WeaponDesc{};
    WeaponDesc.pSocketMatrix = pPartBody->Get_SocketMatrix("SWORD");
        // 그냥 무기 없이 진행하고 싶음
    if (nullptr == WeaponDesc.pSocketMatrix)
    {
        MSG_BOX("Body에서 'SWORD' 소켓을 찾지 못했습니다.");
        return S_OK;
    }

    WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    WeaponDesc.pParentState = &m_iState;

    if (FAILED(__super::Add_PartObject(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Weapon_Player"),
        TEXT("Part_Weapon"), &WeaponDesc)))
        return E_FAIL;

    return S_OK;
}


HRESULT Player::Bind_ShaderResources()
{
	return S_OK;
}

Player* Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Player* pInstance = new Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Player");
        Safe_Release(pInstance);
    }
    return pInstance;
}

GameObject* Player::Clone(void* pArg)
{
    Player* pInstance = new Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void Player::Free()
{
	__super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);
}
