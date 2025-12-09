#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Cell final : public Base
{
private:
	Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Cell() = default;

public:
	_int Get_Index() const {
		return m_iIndex;
	}

	_int Get_ParentIndex() {
		return m_iParentIndex;
	}
	_vector Get_Point(POINT ePoint) const {
		return XMLoadFloat3(&m_vPoints[ENUM_TO_UINT(ePoint)]);
	}

	void Get_Neighbors(_int* pNeighbors) {
		memcpy(pNeighbors, m_iNeighbors, sizeof(_int) * 3);

	}

	_vector Get_CenterPos();

	void Set_Neighbor(LINE eLine, Cell* pNeighbor) {
		m_iNeighbors[ENUM_TO_UINT(eLine)] = pNeighbor->m_iIndex;
	}

	void Set_Neighbors(_int* pNeighbors)
	{
		memcpy(m_iNeighbors, pNeighbors, sizeof(_int) * 3);
	}

	void Set_ParentIndex(_int iParentIndex) {
		m_iParentIndex = iParentIndex;
	}

public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex);
	_bool Compare(_fvector vSourPoint, _fvector vDestPoint);
	_bool isIn(_fvector vResultPos, _int* pNeighborIndex);
	_float Compute_Height(_vector vCellPos);
	_float Compute_Cost(const vector<Cell*>& Cells, _int iGoalIndex);
	void WriteData(HANDLE hFile)
	{
		_ulong		dwByte = {};
		WriteFile(hFile, m_iNeighbors, sizeof(_int) * 3, &dwByte, nullptr);
	}

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	_float3			m_vPoints[ENUM_TO_UINT(POINT::END)] = {};
	_float3			m_vNormals[ENUM_TO_UINT(LINE::END)] = {};
	_int			m_iNeighbors[ENUM_TO_UINT(LINE::END)] = { -1, -1, -1 };
	_uint			m_iIndex; 

private:
	_float			m_fTotalCost;
	_int			m_iParentIndex = { -1 };


#ifdef _DEBUG
private:
	class VIBuffer_Cell* m_pVIBuffer = { nullptr };

#endif

public:
	static Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void Free() override;
};

NS_END