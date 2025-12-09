#include "Light_Manager.h"
#include "Light.h"

Light_Manager::Light_Manager()
{
}

const LIGHT_DESC* Light_Manager::Get_LightDesc(_uint iIndex)
{
    auto    iter = m_Lights.begin();

    for (size_t i = 0; i < iIndex; i++)
        ++iter;

    return (*iter)->Get_LightDesc();    
}



HRESULT Light_Manager::Add_Light(const LIGHT_DESC& LightDesc)
{
    Light* pLight = Light::Create(LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_Lights.push_back(pLight);

    return S_OK;
}

Light_Manager* Light_Manager::Create()
{

    return new Light_Manager();
}

void Light_Manager::Free()
{
    __super::Free();

    for (auto& pLight : m_Lights)
        Safe_Release(pLight);

    m_Lights.clear();
}
