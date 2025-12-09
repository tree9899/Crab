#pragma once

#include "Base.h"

/* 뷰, 투영행렬을 보관해주는 기능. (m_pGraphic_Device->SetTransform(D3DTS_VIEW, &뷰행렬); */
/* 쉐이더에 던져주는 기능. */
/* 매 프레임마다 뷰, 투영의 역행렬을 구해놓는기능.(UPDATE) */
/* 카메라 위치도 구해놓는 기능 */

NS_BEGIN(Engine)

class PipeLine final : public Base
{
private:
	PipeLine();
	virtual ~PipeLine() = default;

public:
	void Set_Transform(D3DTS eTransformMatrix, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrices[ENUM_TO_UINT(eTransformMatrix)], 
			TransformMatrix);	}

	const _float4x4* Get_Transform(D3DTS eTransformMatrix) {
		return &m_TransformMatrices[ENUM_TO_UINT(eTransformMatrix)];
	}

	const _float4* Get_CamPosition() {
		return &m_vCamPosition;
	}
public:
	HRESULT Bind_PipeLineMatrix(class Shader* pShader, const _char* pConstant, D3DTS eTransformMatrix);
	HRESULT Bind_PipeLineInverseMatrix(class Shader* pShader, const _char* pConstant, D3DTS eTransformMatrix);
	HRESULT Bind_CamPosition(class Shader* pShader, const _char* pConstant);

	void Update();

private:
	_float4x4			m_TransformMatrices[ENUM_TO_UINT(D3DTS::END)] = {};
	_float4x4			m_TransformInverseMatrices[ENUM_TO_UINT(D3DTS::END)] = {};
	_float4				m_vCamPosition;

public:
	static PipeLine* Create();
	virtual void Free() override;
};

NS_END