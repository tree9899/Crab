#pragma once

#include "Base.h"

/* Channel :  이 뼈!는 어떤 시간에 어떤 상태를 취한다라는 것들을 저장하기위한거  */
/* KeyFrame : 뼈의 상태! */
/* 상태와 상태 사이를 보간하여 상태를 만들어준다. */
/* 만들어 낸 상태는 CBone의 m_Transformationmatrix에 전달해준다. */

NS_BEGIN(Engine)

class Channel final : public Base
{
private:
	Channel();
	virtual ~Channel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, class Model* pModel);
	void Update_TransformationMatrix(const vector<class Bone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex);

private:
	_uint				m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;
	_int				m_iBoneIndex = {};


public:
	static Channel* Create(const aiNodeAnim* pAIChannel, class Model* pModel);
	virtual void Free() override;
};

NS_END