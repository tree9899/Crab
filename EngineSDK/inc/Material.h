#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Material final : public Base
{
private:
	Material(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Material() = default;

public:
	HRESULT Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath);
	HRESULT Bind_ShaderResource(class Shader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iTextureIndex);
private:
	ID3D11Device*								m_pDevice = { nullptr };
	ID3D11DeviceContext*						m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>			m_SRVs[AI_TEXTURE_TYPE_MAX];

public:
	static Material* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath);
	virtual void Free() override;
};

NS_END