#pragma once

#include "Base.h"

/* 특정 동작(공격, 대기, 걷기 등등 )을 위한 뼈대들의 시간에 따른 상태값(행렬)을 보관한다.  */
/* Channel : 특정 뼈의 시간에 따른 상태값(행렬)을 보관 */

NS_BEGIN(Engine)

class Animation final : public Base
{
private:
	Animation();
	Animation(const Animation& Prototype);
	virtual ~Animation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, class Model* pModel);
	_bool Update_TransformationMatrices(const vector<class Bone*>& Bones, _float fTimeDelta, _bool isLoop);

private:
	/* 이 동작을 구현해주기위해 사용해야하는 뼈의 갯수 */
	_uint					m_iNumChannels = {};
	vector<class Channel*>	m_Channels;
	vector<_uint>			m_CurrentKeyFrameIndices;

	_char					m_szName[MAX_PATH] = {};
	_float					m_fTickPerSecond = {}; /* 애니메이션의 재생 속도 */
	_float					m_fDuration = {}; /* 현재 애니메이션의 전체 재생 길이 */
	_float					m_fCurrentTrackPosition = {};

public:
	static Animation* Create(const aiAnimation* pAIAnimation, class Model* pModel);
	Animation* Clone();
	virtual void Free() override;
};

NS_END