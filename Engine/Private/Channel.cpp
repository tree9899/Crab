#include "Channel.h"
#include "Model.h"
#include "Bone.h"

Channel::Channel()
{
	/*XMMatrixDecompose();*/
}

HRESULT Channel::Initialize(const aiNodeAnim* pAIChannel, Model* pModel)
{
	m_iBoneIndex = pModel->Get_BoneIndex(pAIChannel->mNodeName.data);
	if (-1 == m_iBoneIndex)
		return E_FAIL;

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3		vScale{};
	_float4		vRotation{};
	_float3		vTranslation{};

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME			KeyFrame{};

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTrackPosition = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;

			KeyFrame.fTrackPosition = (_float)pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTrackPosition = (_float)pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vTranslation = vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void Channel::Update_TransformationMatrix(const vector<Bone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex)
{
	if (0.0f == fCurrentTrackPosition)
		(*pCurrentKeyFrameIndex)= 0;

	/* fCurrentTrackPosition시간에 맞는 현재 뼈의 상태를 만든다.*/
	_float4x4	TransformationMatrix = {};

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	_vector		vScale, vRotation, vTranslation;

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}
	else
	{
		_vector		vLeftScale{}, vRightScale{};
		_vector		vLeftRotation{}, vRightRotation{};
		_vector		vLeftTranslation{}, vRightTranslation{};

		while (fCurrentTrackPosition >= m_KeyFrames[(*pCurrentKeyFrameIndex)+ 1].fTrackPosition)
			++(*pCurrentKeyFrameIndex);

		vLeftScale = XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex)].vScale);
		vRightScale = XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex)+ 1].vScale);

		vLeftRotation = XMLoadFloat4(&m_KeyFrames[(*pCurrentKeyFrameIndex)].vRotation);
		vRightRotation = XMLoadFloat4(&m_KeyFrames[(*pCurrentKeyFrameIndex)+ 1].vRotation);

		vLeftTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex)].vTranslation), 1.f);
		vRightTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex)+ 1].vTranslation), 1.f);

		_float		fRatio = (fCurrentTrackPosition - m_KeyFrames[(*pCurrentKeyFrameIndex)].fTrackPosition) /
			(m_KeyFrames[(*pCurrentKeyFrameIndex)+ 1].fTrackPosition - m_KeyFrames[(*pCurrentKeyFrameIndex)].fTrackPosition);

		vScale = XMVectorLerp(vLeftScale, vRightScale, fRatio);
		vRotation = XMQuaternionSlerp(vLeftRotation, vRightRotation, fRatio);
		vTranslation = XMVectorLerp(vLeftTranslation, vRightTranslation, fRatio);
	}

	/*XMStoreFloat4x4(&TransformationMatrix,
		XMMatrixScaling() * XMMatrixRotationQuaternion() * XMMatrixTranslation());*/

	XMStoreFloat4x4(&TransformationMatrix,
		XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);	
}

Channel* Channel::Create(const aiNodeAnim* pAIChannel, class Model* pModel)
{
	Channel* pInstance = new Channel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : Channel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Channel::Free()
{
	__super::Free();

	m_KeyFrames.clear();
}
