#include "VIBuffer_Cell.h"

VIBuffer_Cell::VIBuffer_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: VIBuffer { pDevice, pContext }
{
}

VIBuffer_Cell::VIBuffer_Cell(const VIBuffer_Cell& Prototype)
	: VIBuffer{ Prototype }
{
}

HRESULT VIBuffer_Cell::Initialize_Prototype(const _float3* pPoints)
{
    m_iVertexStride = sizeof(VTXPOS);
    m_iNumVertices = 3;
    m_iIndexStride = 2;

    m_iNumIndices = 4;
    m_iNumVertexBuffers = 1;
    m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

#pragma region VERTEX_BUFFER
    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;

    VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

    m_pVertexPositions = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

    memcpy(pVertices, pPoints, sizeof(VTXPOS) * m_iNumVertices);
    memcpy(m_pVertexPositions, pPoints, sizeof(VTXPOS) * m_iNumVertices);

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

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;
    pIndices[3] = 0;

    D3D11_SUBRESOURCE_DATA      IndexInitialData{};
    IndexInitialData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT VIBuffer_Cell::Initialize(void* pArg)
{
	return S_OK;
}

VIBuffer_Cell* VIBuffer_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints)
{
    VIBuffer_Cell* pInstance = new VIBuffer_Cell(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pPoints)))
    {
        MSG_BOX("Failed to Created : VIBuffer_Cell");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Component* VIBuffer_Cell::Clone(void* pArg)
{
    VIBuffer_Cell* pInstance = new VIBuffer_Cell(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : VIBuffer_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void VIBuffer_Cell::Free()
{
	__super::Free();
}
