#include "Material.h"
#include "Shader.h"

Material::Material(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Material::Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		_uint		iNumTextures = pAIMaterial->GetTextureCount(static_cast<aiTextureType>(i));

		_char		szTextureFilePath[MAX_PATH] = {};
		_char		szDrive[MAX_PATH] = {};
		_char		szDir[MAX_PATH] = {};

		_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);


		for (size_t j = 0; j < iNumTextures; j++)
		{
			aiString	strTextureFilePath = {};

			if (FAILED(pAIMaterial->GetTexture(static_cast<aiTextureType>(i), (unsigned int)j, &strTextureFilePath)))
				return E_FAIL;
			
			_char		szFileName[MAX_PATH] = {};
			_char		szExt[MAX_PATH] = {};

			
			strcpy_s(szTextureFilePath, szDrive);			
			strcat_s(szTextureFilePath, szDir);

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
			strcat_s(szTextureFilePath, szFileName);
			strcat_s(szTextureFilePath, szExt);

			_tchar		szFinalPath[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, szTextureFilePath, (int)strlen(szTextureFilePath), szFinalPath, MAX_PATH);


			HRESULT         hr = {};
			ID3D11ShaderResourceView* pSRV = { nullptr };

			
			if (false == strcmp(szExt, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, szFinalPath, nullptr, &pSRV);

			else if (false == strcmp(szExt, ".tga"))
				return E_FAIL;

			else
				hr = CreateWICTextureFromFile(m_pDevice, szFinalPath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_SRVs[i].push_back(pSRV);
		}
	}

    return S_OK;
}

HRESULT Material::Bind_ShaderResource(Shader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName, m_SRVs[eMaterialType][iTextureIndex]);	
}

Material* Material::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	Material* pInstance = new Material(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pAIMaterial, pModelFilePath)))
	{
		MSG_BOX("Failed to Created : Material");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Material::Free()
{
	__super::Free();

	for (auto& SRVs : m_SRVs)
	{
		for (auto& pSRV : SRVs)
			Safe_Release(pSRV);

		SRVs.clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
