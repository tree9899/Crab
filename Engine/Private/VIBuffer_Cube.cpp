#include "VIBuffer_Cube.h"

VIBuffer_Cube::VIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: VIBuffer { pDevice, pContext }
{
}

VIBuffer_Cube::VIBuffer_Cube(const VIBuffer_Cube& Prototype)
	: VIBuffer{ Prototype }
{
}

HRESULT VIBuffer_Cube::Initialize_Prototype()
{
    m_iVertexStride = sizeof(VTXCUBE);
    m_iNumVertices = 8;
    m_iIndexStride = 2;

    m_iNumIndices = 36;
    m_iNumVertexBuffers = 1;
    m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;

    VTXCUBE* pVertices = new VTXCUBE[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXCUBE) * m_iNumVertices);

    m_pVertexPositions = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

    m_pVertexPositions[0] = pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
    pVertices[0].vTexcoord = m_pVertexPositions[0];

    m_pVertexPositions[1] = pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
    pVertices[1].vTexcoord = m_pVertexPositions[1];

    m_pVertexPositions[2] = pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
    pVertices[2].vTexcoord = m_pVertexPositions[2];

    m_pVertexPositions[3] = pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
    pVertices[3].vTexcoord = m_pVertexPositions[3];

    m_pVertexPositions[4] = pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
    pVertices[4].vTexcoord = m_pVertexPositions[4];

    m_pVertexPositions[5] = pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
    pVertices[5].vTexcoord = m_pVertexPositions[5];

    m_pVertexPositions[6] = pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
    pVertices[6].vTexcoord = m_pVertexPositions[6];

    m_pVertexPositions[7] = pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
    pVertices[7].vTexcoord = m_pVertexPositions[7];

    D3D11_SUBRESOURCE_DATA      VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
    D3D11_BUFFER_DESC           IndexBufferDesc{};
    IndexBufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.CPUAccessFlags = 0;
    IndexBufferDesc.MiscFlags = 0;
    IndexBufferDesc.StructureByteStride = m_iIndexStride;

    _ushort* pIndices = new _ushort[m_iNumIndices];

    pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
    pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;

    pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
    pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;


    pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
    pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;


    pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
    pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;


    pIndices[24] = 7; pIndices[25] = 6; pIndices[26] = 5;
    pIndices[27] = 7; pIndices[28] = 5; pIndices[29] = 4;


    pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
    pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;


    D3D11_SUBRESOURCE_DATA      IndexInitialData{};
    IndexInitialData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT VIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

VIBuffer_Cube* VIBuffer_Cube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    VIBuffer_Cube* pInstance = new VIBuffer_Cube(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : VIBuffer_Cube");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Component* VIBuffer_Cube::Clone(void* pArg)
{
    VIBuffer_Cube* pInstance = new VIBuffer_Cube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : VIBuffer_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void VIBuffer_Cube::Free()
{
	__super::Free();
}
