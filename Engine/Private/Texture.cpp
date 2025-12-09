#include "Texture.h"
#include "Shader.h"

Texture::Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice, pContext }
{
}

Texture::Texture(const Texture& Prototype)
    : Component{ Prototype }
    , m_iNumTextures { Prototype.m_iNumTextures }
    , m_Textures { Prototype.m_Textures }
{
    for (auto& pSRV : m_Textures)
        Safe_AddRef(pSRV);
}

HRESULT Texture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
    m_iNumTextures = iNumTextures;

    /* D:\Burger\156\Framework\Engine\Private\Test.jpg */

    /* D:\ */
    _tchar      szDrive[MAX_PATH] = {};

    /* Burger\156\Framework\Engine\Private\ */
    _tchar      szDir[MAX_PATH] = {};

    /* .jpg */
    _tchar      szEXT[MAX_PATH] = {};
        
    _wsplitpath_s(pTextureFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, szEXT, MAX_PATH);

    for (size_t i = 0; i < iNumTextures; i++)
    {
        ID3D11ShaderResourceView* pSRV = { nullptr };
        _tchar      szFilePath[MAX_PATH] = {};

        wsprintf(szFilePath, pTextureFilePath, i);

        
        HRESULT         hr = {};

        if (false == lstrcmp(szEXT, TEXT(".dds")))
            hr = CreateDDSTextureFromFile(m_pDevice, szFilePath, nullptr, &pSRV);

        else if (false == lstrcmp(szEXT, TEXT(".tga")))
            return E_FAIL;

        else
            hr = CreateWICTextureFromFile(m_pDevice, szFilePath, nullptr, &pSRV);

        if (FAILED(hr))
            return E_FAIL;        

        m_Textures.push_back(pSRV);
    }

    return S_OK;
}

HRESULT Texture::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Texture::Bind_ShaderResource(Shader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
    if (iTextureIndex >= m_iNumTextures)
        return E_FAIL;

    return pShader->Bind_SRV(pConstantName, m_Textures[iTextureIndex]);   
}

HRESULT Texture::Bind_ShaderResources(Shader* pShader, const _char* pConstantName)
{
    return pShader->Bind_SRVs(pConstantName, &m_Textures.front(), m_iNumTextures);
}

Texture* Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
    Texture* pInstance = new Texture(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
    {
        MSG_BOX("Failed to Created : Texture");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Component* Texture::Clone(void* pArg)
{
    Texture* pInstance = new Texture(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Texture");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Texture::Free()
{
    __super::Free();

    for (auto& pSRV : m_Textures)
        Safe_Release(pSRV);
    m_Textures.clear();

}
