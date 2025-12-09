#pragma once

#include "Component.h"

/* 정점과 인덱스 버퍼를 만드는 모든 클래스들의 부모클래스다. */

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer abstract : public Component
{
protected:
	VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer(const VIBuffer& Prototype);
	virtual ~VIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Bind_Resource();
	virtual HRESULT Render();

protected:
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

protected:
	_float3*					m_pVertexPositions = { nullptr };
	_uint						m_iNumVertices = {};
	_uint						m_iVertexStride = {};
	_uint						m_iNumIndices = {};
	_uint						m_iIndexStride = {};
	_uint						m_iNumVertexBuffers = {};
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitiveType = {};


public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END