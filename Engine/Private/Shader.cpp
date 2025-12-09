#include "Shader.h"

Shader::Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice, pContext }
{
}

Shader::Shader(const Shader& Prototype)
    : Component{ Prototype }
    , m_pEffect { Prototype.m_pEffect }
    , m_InputLayouts{Prototype.m_InputLayouts}    
{
    for (auto& pInputLayout : m_InputLayouts)
        Safe_AddRef(pInputLayout);

    Safe_AddRef(m_pEffect);
}

HRESULT Shader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    _uint       iHlslFlag = {};


#ifdef _DEBUG
    iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif   

    if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
        return E_FAIL;

    ID3DX11EffectTechnique*     pTechnique = m_pEffect->GetTechniqueByIndex(0);
    if (nullptr == pTechnique)
        return E_FAIL;

    D3DX11_TECHNIQUE_DESC       TechniqueDesc{};

    pTechnique->GetDesc(&TechniqueDesc);

    m_iNumPasses = TechniqueDesc.Passes;

    for (size_t i = 0; i < m_iNumPasses; i++)
    {
        ID3D11InputLayout* pInputLayout = { nullptr };

        ID3DX11EffectPass*  pPass = pTechnique->GetPassByIndex((uint32_t)i);

        D3DX11_PASS_DESC    PassDesc{};

        pPass->GetDesc(&PassDesc);

        if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements,
            PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
            return E_FAIL;

        m_InputLayouts.push_back(pInputLayout);
    }   

    return S_OK;
}

HRESULT Shader::Initialize(void* pArg)
{
    return S_OK;
}



HRESULT Shader::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    return pVariable->SetRawValue(pData, 0, iLength);    
}

HRESULT Shader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
    ID3DX11EffectVariable*  pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable*    pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));    
}

HRESULT Shader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(pMatrix), 0, iNumMatrices);
}

HRESULT Shader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* pSRVVariable = pVariable->AsShaderResource();
    if (nullptr == pSRVVariable)
        return E_FAIL;

    return pSRVVariable->SetResource(pSRV);
}

HRESULT Shader::Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumSRVs)
{

    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* pSRVVariable = pVariable->AsShaderResource();
    if (nullptr == pSRVVariable)
        return E_FAIL;

    return pSRVVariable->SetResourceArray(ppSRVs, 0, iNumSRVs);
}

HRESULT Shader::Begin(_uint iPassIndex)
{
    if (nullptr == m_InputLayouts[iPassIndex])
        return E_FAIL;

    m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

    m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);

    return S_OK;
}

Shader* Shader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    Shader* pInstance = new Shader(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
    {
        MSG_BOX("Failed to Created : Shader");
        Safe_Release(pInstance);
    }
    return pInstance;   
}

Component* Shader::Clone(void* pArg)
{
    Shader* pInstance = new Shader(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Shader");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void Shader::Free()
{
    __super::Free();

    for (auto& pInputLayout : m_InputLayouts)
        Safe_Release(pInputLayout);
    m_InputLayouts.clear();

    Safe_Release(m_pEffect);


}
