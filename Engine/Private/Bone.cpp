#include "Bone.h"

Bone::Bone()
{
}

HRESULT Bone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
    strcpy_s(m_szName, pAINode->mName.data);    

    m_iParentBoneIndex = iParentBoneIndex;

    memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

    XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	return S_OK;
}

void Bone::Update_CombinedTransformationMatrix(const vector<Bone*>& Bones, _fmatrix PreTransformMatrix)
{
    if (-1 == m_iParentBoneIndex)
    {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix,
            XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
    }
    else 
    {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix,
            XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
    }

}

Bone* Bone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
    Bone* pInstance = new Bone();

    if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
    {
        MSG_BOX("Failed to Created : Bone");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Bone* Bone::Clone()
{
    return new Bone(*this);
}


void Bone::Free()
{
	__super::Free();


}
