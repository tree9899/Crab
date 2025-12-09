#include "PipeLine.h"
#include "Shader.h"

PipeLine::PipeLine()
{
}

HRESULT PipeLine::Bind_PipeLineMatrix(Shader* pShader, const _char* pConstantName, D3DTS eTransformMatrix)
{
    return pShader->Bind_Matrix(pConstantName, &m_TransformMatrices[ENUM_TO_UINT(eTransformMatrix)]);    
}

HRESULT PipeLine::Bind_PipeLineInverseMatrix(Shader* pShader, const _char* pConstantName, D3DTS eTransformMatrix)
{
    return pShader->Bind_Matrix(pConstantName, &m_TransformInverseMatrices[ENUM_TO_UINT(eTransformMatrix)]);
}

HRESULT PipeLine::Bind_CamPosition(Shader* pShader, const _char* pConstantName)
{
    return pShader->Bind_RawValue(pConstantName, &m_vCamPosition, sizeof(_float4));
    
}

void PipeLine::Update()
{
    for (size_t i = 0; i < ENUM_TO_UINT(D3DTS::END); i++)
    {
        XMStoreFloat4x4(&m_TransformInverseMatrices[i], 
            XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrices[i])));
    }

    memcpy(&m_vCamPosition,
        &m_TransformInverseMatrices[ENUM_TO_UINT(D3DTS::VIEW)].m[3], sizeof(_float4));    
}

PipeLine* PipeLine::Create()
{
    return new PipeLine();
}

void PipeLine::Free()
{
    __super::Free();


}
