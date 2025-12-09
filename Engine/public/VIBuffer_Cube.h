#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Cube final : public VIBuffer
{
private:
	VIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer_Cube(const VIBuffer_Cube& Prototype);
	virtual ~VIBuffer_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static VIBuffer_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END