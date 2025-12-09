#include "Cell.h"
#include "VIBuffer_Cell.h"

Cell::Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_vector Cell::Get_CenterPos()
{

	_vector		vPosition = {};

	for (size_t i = 0; i < 3; i++)
	{
		vPosition += XMLoadFloat3(&m_vPoints[i]);
	}

	return vPosition / 3.f;	
}

HRESULT Cell::Initialize(const _float3* pPoints, _int iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * ENUM_TO_UINT(POINT::END));

	m_iIndex = iIndex;

	_vector		vLine = {};

	vLine = XMVector3Normalize(XMVectorSetY(XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::B)]) - XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::A)]), 0.f));
	m_vNormals[ENUM_TO_UINT(LINE::AB)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMVector3Normalize(XMVectorSetY(XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::C)]) - XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::B)]), 0.f));
	m_vNormals[ENUM_TO_UINT(LINE::BC)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMVector3Normalize(XMVectorSetY(XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::A)]) - XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::C)]), 0.f));
	m_vNormals[ENUM_TO_UINT(LINE::CA)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

#ifdef _DEBUG
	m_pVIBuffer = VIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

_bool Cell::Compare(_fvector vSourPoint, _fvector vDestPoint)
{
	/*XMVectorEqual();*/
	if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::A)])))
	{
		if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::B)])))		
			return true;

		if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::C)])))
			return true;	
	}

	if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::B)])))
	{
		if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::C)])))
			return true;

		if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::A)])))
			return true;
	}

	if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::C)])))
	{
		if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::A)])))
			return true;

		if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::B)])))
			return true;
	}

	return false;
}

_bool Cell::isIn(_fvector vResultPos, _int* pNeighborIndex)
{
	for (size_t i = 0; i < ENUM_TO_UINT(LINE::END); i++)
	{
		_vector		vDir = XMVector3Normalize(vResultPos - XMLoadFloat3(&m_vPoints[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, XMLoadFloat3(&m_vNormals[i]))))
		{
			*pNeighborIndex = m_iNeighbors[i];

			return false;
		}
	}

	return true;
}

_float Cell::Compute_Height(_vector vCellPos)
{	
	// XMPlaneFromPointNormal();

	_vector		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::A)]),
		XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::B)]),
		XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(POINT::C)]));	

	// ax + by + cz + d = 0
	// y = (-ax - cz - d) / b 

	return (-XMVectorGetX(vPlane) * XMVectorGetX(vCellPos) - XMVectorGetZ(vPlane) * XMVectorGetZ(vCellPos) - XMVectorGetW(vPlane)) 
		/ XMVectorGetY(vPlane);;




	

	return _float();
}

_float Cell::Compute_Cost(const vector<Cell*>& Cells, _int iGoalIndex)
{
	_float		fPCost = {};
	if(-1 != m_iParentIndex)
		fPCost = XMVector3Length(Cells[m_iParentIndex]->Get_CenterPos() - Get_CenterPos()).m128_f32[0];

	_float		fGCost = XMVector3Length(Cells[iGoalIndex]->Get_CenterPos() - Get_CenterPos()).m128_f32[0];

	return fPCost + fGCost;
}

#ifdef _DEBUG

HRESULT Cell::Render()
{
	if (FAILED(m_pVIBuffer->Bind_Resource()))
		return E_FAIL;

	m_pVIBuffer->Render();

	return S_OK;
}

#endif

Cell* Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
	Cell* pInstance = new Cell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : Cell");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Cell::Free()
{
	__super::Free();


#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
