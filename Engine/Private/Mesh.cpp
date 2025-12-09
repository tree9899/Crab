#include "Mesh.h"
#include "Model.h"
#include "Bone.h"
#include "Shader.h"

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : VIBuffer { pDevice, pContext }
{
}

Mesh::Mesh(const Mesh& Prototype)
    : VIBuffer { Prototype }
{
}

HRESULT Mesh::Initialize_Prototype(MODEL eModelType, const aiMesh* pAIMesh, Model* pModel, _fmatrix PreTransformMatrix)
{
    strcpy_s(m_szName, pAIMesh->mName.data);

    m_iMaterialIndex = pAIMesh->mMaterialIndex;
    
    m_iNumVertices = pAIMesh->mNumVertices;
    m_iIndexStride = 4;

    m_iNumIndices = pAIMesh->mNumFaces * 3;
    m_iNumVertexBuffers = 1;
    m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
    HRESULT hr = MODEL::NONANIM == eModelType ? 
        Ready_For_NonAnimMesh(pAIMesh, PreTransformMatrix) : 
        Ready_For_AnimMesh(pAIMesh, pModel);
    if (FAILED(hr))
        return E_FAIL;    

#pragma endregion

#pragma region INDEX_BUFFER
    D3D11_BUFFER_DESC           IndexBufferDesc{};
    IndexBufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.CPUAccessFlags = 0;
    IndexBufferDesc.MiscFlags = 0;
    IndexBufferDesc.StructureByteStride = m_iIndexStride;

    _uint* pIndices = new _uint[m_iNumIndices];

    _uint   iNumIndices = {};

    for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
    {
        pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
        pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
        pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
    }  

    D3D11_SUBRESOURCE_DATA      IndexInitialData{};
    IndexInitialData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);

#pragma endregion

    return S_OK;
}

HRESULT Mesh::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Mesh::Bind_Bones(Shader* pShader, const _char* pConstantName, const vector<Bone*>& Bones)
{
    for (size_t i = 0; i < m_iNumBones; i++)
    {
        XMStoreFloat4x4(&m_BoneMatrices[i], 
            XMLoadFloat4x4(&m_OffsetMatrices[i]) * 
            Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
    }

    return pShader->Bind_Matrices(pConstantName, m_BoneMatrices, g_iMaxNumBones);
}

HRESULT Mesh::Ready_For_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
    m_iVertexStride = sizeof(VTXMESH);

    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;

    VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

    m_pVertexPositions = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

    for (size_t i = 0; i < m_iNumVertices; i++)
    {
        memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
        XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

        m_pVertexPositions[i] = pVertices[i].vPosition;

        memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
        XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

        memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
        XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix));

        memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
        XMStoreFloat3(&pVertices[i].vBinormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vBinormal), PreTransformMatrix));

        memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
    }

    D3D11_SUBRESOURCE_DATA      VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);

    return S_OK;
}

HRESULT Mesh::Ready_For_AnimMesh(const aiMesh* pAIMesh, Model* pModel)
{
    
    m_iVertexStride = sizeof(VTXANIMMESH);

    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;

    VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

    m_pVertexPositions = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

    for (size_t i = 0; i < m_iNumVertices; i++)
    {
        memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
        m_pVertexPositions[i] = pVertices[i].vPosition;

        memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
        memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));            
    }

    m_iNumBones = pAIMesh->mNumBones; 

    for (size_t i = 0; i < m_iNumBones; i++)
    {
        aiBone* pAIBone = pAIMesh->mBones[i];

        _uint   iBoneIndex = pModel->Get_BoneIndex(pAIBone->mName.data);
        if (-1 == iBoneIndex)
            return E_FAIL;

        m_BoneIndices.push_back(iBoneIndex);

        _float4x4       OffsetMatrix = {};
        memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

        XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

        m_OffsetMatrices.push_back(OffsetMatrix);

        /* i번째 뼈는 몇개의 정점에게 영향을 주는가? */
        for (size_t j = 0; j < pAIBone->mNumWeights; j++)
        {
            aiVertexWeight  AIWeight = pAIBone->mWeights[j];

            /* AIWeight.mVertexId : i번째 뼈가 영향을 주는 j번째 정점의 인덱스 */
            if (0.0f == pVertices[AIWeight.mVertexId].vBlendWeight.x)
            {
                pVertices[AIWeight.mVertexId].vBlendIndex.x = (int)i;
                pVertices[AIWeight.mVertexId].vBlendWeight.x = AIWeight.mWeight;
            }

            else if (0.0f == pVertices[AIWeight.mVertexId].vBlendWeight.y)
            {
                pVertices[AIWeight.mVertexId].vBlendIndex.y = (int)i;
                pVertices[AIWeight.mVertexId].vBlendWeight.y = AIWeight.mWeight;
            }

            else if (0.0f == pVertices[AIWeight.mVertexId].vBlendWeight.z)
            {
                pVertices[AIWeight.mVertexId].vBlendIndex.z = (int)i;
                pVertices[AIWeight.mVertexId].vBlendWeight.z = AIWeight.mWeight;
            }

            else
            {
                pVertices[AIWeight.mVertexId].vBlendIndex.w = (int)i;
                pVertices[AIWeight.mVertexId].vBlendWeight.w = AIWeight.mWeight;
            }
        }
    }

    if (0 == m_iNumBones)
    {
        m_iNumBones = 1;

        _float4x4       OffsetMatrix;
        XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

        m_OffsetMatrices.push_back(OffsetMatrix);
        m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));
    }

    D3D11_SUBRESOURCE_DATA      VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);



    return S_OK;
}

Mesh* Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const aiMesh* pAIMesh, Model* pModel, _fmatrix PreTransformMatrix)
{
    Mesh* pInstance = new Mesh(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, pModel, PreTransformMatrix)))
    {
        MSG_BOX("Failed to Created : Mesh");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Component* Mesh::Clone(void* pArg)
{
    Mesh* pInstance = new Mesh(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Mesh");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Mesh::Free()
{
    __super::Free();

}
