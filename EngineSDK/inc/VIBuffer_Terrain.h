#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Terrain final : public VIBuffer
{
private:
	VIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer_Terrain(const VIBuffer_Terrain& Prototype);
	virtual ~VIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightFileMapPath);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint		m_iNumVerticesX = { };
	_uint		m_iNumVerticesZ = { };
public:
	static VIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightFileMapPath);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END