#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Cell final : public VIBuffer
{
private:
	VIBuffer_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer_Cell(const VIBuffer_Cell& Prototype);
	virtual ~VIBuffer_Cell() = default;

public:
	virtual HRESULT Initialize_Prototype(const _float3* pPoints);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static VIBuffer_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END