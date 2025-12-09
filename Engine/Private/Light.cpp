#include "Light.h"

Light::Light()
{
}

HRESULT Light::Initialize(const LIGHT_DESC& LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

Light* Light::Create(const LIGHT_DESC& LightDesc)
{
	Light* pInstance = new Light();

	/* 메인앱을 완벽히 사용하기 위한 기타 초기화작업을 수행한다. */
	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Created : Light");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void Light::Free()
{
	__super::Free();

	
}
