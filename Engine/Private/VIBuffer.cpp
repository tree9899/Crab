#include "VIBuffer.h"

VIBuffer::VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Component { pDevice, pContext }
{
}

VIBuffer::VIBuffer(const VIBuffer& Prototype)
	: Component{ Prototype }
	, m_pVB { Prototype.m_pVB }
	, m_pIB { Prototype.m_pIB }
	, m_iNumVertices { Prototype.m_iNumVertices }
	, m_iVertexStride { Prototype.m_iVertexStride }
	, m_pVertexPositions { Prototype.m_pVertexPositions }
	, m_iNumIndices{ Prototype.m_iNumIndices }
	, m_iIndexStride { Prototype.m_iIndexStride}
	, m_iNumVertexBuffers { Prototype.m_iNumVertexBuffers }
	, m_ePrimitiveType { Prototype.m_ePrimitiveType }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT VIBuffer::Initialize_Prototype()
{
	/*D3D11_BUFFER_DESC{};
	D3D11_SUBRESOURCE_DATA{};

	m_pDevice->CreateBuffer();*/

	return S_OK;
}

HRESULT VIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT VIBuffer::Bind_Resource()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB, 
		/*m_pVBInstance,*/
	};

	_uint		 iVertexStrides[] = {
		m_iVertexStride,
	};

	_uint		 iOffsets[] = {
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveType);
	

	/* VTXPOSTEX */
	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}		
	};

	//ID3D11InputLayout* pInputLayout = { nullptr };
	//m_pDevice->CreateInputLayout(Elements, 2,
	//	쉐이더에서는 어떻게 생겨먹은 정점을 받는데?, 정점의 멤버 갯수, &pInputLayout);


	//m_pContext->IASetInputLayout(pInputLayout);


	return S_OK;
}

HRESULT VIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

void VIBuffer::Free()
{
	__super::Free();

	if (false == m_isClone)
		Safe_Delete_Array(m_pVertexPositions);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
