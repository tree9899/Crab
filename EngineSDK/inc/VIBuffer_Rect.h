#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Rect final : public VIBuffer
{
private:
	VIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer_Rect(const VIBuffer_Rect& Prototype);
	virtual ~VIBuffer_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static VIBuffer_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END