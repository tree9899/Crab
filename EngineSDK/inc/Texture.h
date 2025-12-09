#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL Texture final : public Component
{
private:
	Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Texture(const Texture& Prototype);
	virtual ~Texture() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_ShaderResource(class Shader* pShader, const _char* pConstantName, _uint iTextureIndex);
	HRESULT Bind_ShaderResources(class Shader* pShader, const _char* pConstantName);

private:
	_uint										m_iNumTextures;
	vector<ID3D11ShaderResourceView*>			m_Textures;	

public:
	static Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END