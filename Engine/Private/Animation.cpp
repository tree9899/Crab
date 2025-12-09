#include "Animation.h"
#include "channel.h"

Animation::Animation()
{

	
}

Animation::Animation(const Animation& Prototype)
	: m_iNumChannels { Prototype.m_iNumChannels }
	, m_Channels { Prototype.m_Channels }
	, m_fTickPerSecond{ Prototype.m_fTickPerSecond }
	, m_fDuration{ Prototype.m_fDuration }
	, m_fCurrentTrackPosition{ Prototype.m_fCurrentTrackPosition }
	, m_CurrentKeyFrameIndices { Prototype.m_CurrentKeyFrameIndices }
{
	strcpy_s(m_szName, Prototype.m_szName);

	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

HRESULT Animation::Initialize(const aiAnimation* pAIAnimation, Model* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);
	m_fDuration = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;
	
	/* 이 애니메이션을 위해 컨트롤해야하는 뼈의 갯수 */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	/* 각 뼈가 시간에 따라 어떻게 움직일지에 대한 정보를 채널 객체에 저장하자. */
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		Channel* pChannel = Channel::Create(pAIAnimation->mChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool Animation::Update_TransformationMatrices(const vector<class Bone*>& Bones, _float fTimeDelta, _bool isLoop)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		if (false == isLoop)
		{

			return true;
		}

		m_fCurrentTrackPosition = 0.f;

		
	}

	_uint	iIndex = {};

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(Bones, m_fCurrentTrackPosition, &m_CurrentKeyFrameIndices[iIndex++]);
	}

	return false;
}

Animation* Animation::Create(const aiAnimation* pAIAnimation, Model* pModel)
{
	Animation* pInstance = new Animation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : Animation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Animation* Animation::Clone()
{
	return new Animation(*this);
}

void Animation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}