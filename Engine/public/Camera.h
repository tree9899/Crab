#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL Camera abstract : public GameObject
{
public:
	typedef struct tagCameraDesc : public GameObject::GAMEOBJECT_DESC
	{
		_float3		vPosition;
		_float3		vAt;

		_float		fFovy;
		_float		fNearZ;
		_float		fFarZ;
	}CAMERA_DESC;
protected:
	Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Camera(const Camera& Prototype); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update_Priority(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Update_Late(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float			m_fFovy = {};
	_float			m_fAspect = {};
	_float			m_fNearZ{}, m_fFarZ{};

protected:
	void Update_PipeLine();


public:
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END