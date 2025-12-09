#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Material.h"
#include "Animation.h"

Model::Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Component { pDevice, pContext }
{
}

Model::Model(const Model& Prototype)
	: Component{ Prototype }
    , m_eModelType { Prototype.m_eModelType }
    , m_iNumMeshes { Prototype.m_iNumMeshes }
    , m_Meshes { Prototype.m_Meshes }
    , m_PreTransformMatrix { Prototype.m_PreTransformMatrix }
    , m_iNumMaterials { Prototype.m_iNumMaterials }
    , m_Materials { Prototype.m_Materials }
    // , m_Bones { Prototype.m_Bones }
    // , m_Animations { Prototype.m_Animations }

{
    for (auto& pPrototypeAnimation : Prototype.m_Animations)
        m_Animations.push_back(pPrototypeAnimation->Clone());

    for (auto& pPrototypeBone : Prototype.m_Bones)
        m_Bones.push_back(pPrototypeBone->Clone());

    for (auto& pMaterial : m_Materials)
        Safe_AddRef(pMaterial);

    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);
}




_int Model::Get_BoneIndex(const _char* pBoneName) const
{
    _int       iIndex = { 0 };

    auto    iter = find_if(m_Bones.begin(), m_Bones.end(), [&](Bone* pBone)->_bool
    {
        if (pBone->Compare_Name(pBoneName))
            return true;

        ++iIndex;

        return false;
    });

    if (iter == m_Bones.end())
        return -1;

    return iIndex;
}

const _float4x4* Model::Get_BoneMatrixPtr(const _char* pBoneName) const
{
    auto    iter = find_if(m_Bones.begin(), m_Bones.end(), [&](Bone* pBone)->_bool
    {
        if (pBone->Compare_Name(pBoneName))
            return true;    

        return false;
    });

    if(iter == m_Bones.end())
        return nullptr;

    return (*iter)->Get_CombinedTransformationMatrixPtr();
}

void Model::Set_Animation(_uint iAnimationIndex, _bool isLoop)
{
    m_iCurrentAnimIndex = iAnimationIndex;
    m_isAnimLoop = isLoop;
}

HRESULT Model::Initialize_Prototype(MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{       
    _uint      iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

    if (MODEL::NONANIM == eModelType)
        iFlag |= aiProcess_PreTransformVertices;

    m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
    if (nullptr == m_pAIScene)
        return E_FAIL; 

    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

    m_eModelType = eModelType;

    if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
        return E_FAIL;

    if (FAILED(Ready_Meshes()))
        return E_FAIL;

    if (FAILED(Ready_Materials(pModelFilePath)))
        return E_FAIL;

    if (FAILED(Ready_Animations()))
        return E_FAIL;

    return S_OK;
}

HRESULT Model::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Model::Bind_Material(Shader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eMaterialType, _uint iTextureIndex)
{
    if(iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    _uint       iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
    if (iMaterialIndex >= m_iNumMaterials)
        return E_FAIL;

    Material* pMaterial = m_Materials[iMaterialIndex]; //추가

    if (nullptr == pMaterial)                           //추가
        return S_OK;

    return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eMaterialType, iTextureIndex);
}

HRESULT Model::Bind_Bones(Shader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    return m_Meshes[iMeshIndex]->Bind_Bones(pShader, pConstantName, m_Bones);    
}

void Model::Play_Animation(_float fTimeDelta)
{
    /* m_iCurrentAnimIndex에 해당하는 애니메이션 중, 현재 재생 시간에 맞는 상태행렬(TransformationMatrix)을 실제 뼈에게 전달해 준다.  */
    m_isAnimFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, fTimeDelta, m_isAnimLoop);

    /* 위에서 갱신해준 뼈들의 TrnasformationMatrix를 기반으로하여 실제 뼈의 상태(CombinedTransformationMatrix)행렬을 만들어준다. */
    for (auto& pBone : m_Bones)
    {
        pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
    }
}

HRESULT Model::Render(_uint iMeshIndex)
{         
    m_Meshes[iMeshIndex]->Bind_Resource();
    m_Meshes[iMeshIndex]->Render();


    return S_OK;
}

HRESULT Model::Ready_Meshes()
{
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        Mesh* pMesh = Mesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
    }   

    return S_OK;
}

HRESULT Model::Ready_Materials(const _char* pModelFilePath)
{
    m_iNumMaterials = m_pAIScene->mNumMaterials;

    if (m_iNumMaterials == 0) //추가
        return S_OK;

    m_Materials.reserve(m_iNumMaterials);

    for (size_t i = 0; i < m_iNumMaterials; i++)
    {
        Material* pMaterial = Material::Create(m_pDevice, m_pContext, m_pAIScene->mMaterials[i], pModelFilePath);
        //if (nullptr == pMaterial)  추가
        //    return E_FAIL;

        m_Materials.push_back(pMaterial);
    }

    return S_OK;
}

HRESULT Model::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
    Bone* pBone = Bone::Create(pAINode, iParentIndex);
    if (nullptr == pBone)
        return E_FAIL;

    m_Bones.push_back(pBone);

    _int        iPIndex = (int)m_Bones.size() - 1;

    for (_uint i = 0; i < pAINode->mNumChildren; ++i)
    {
        Ready_Bones(pAINode->mChildren[i], iPIndex);
    }

    return S_OK;
}

HRESULT Model::Ready_Animations()
{
    m_iNumAnimations = m_pAIScene->mNumAnimations;

    for (size_t i = 0; i < m_iNumAnimations; i++)
    {
        Animation* pAnimation = Animation::Create(m_pAIScene->mAnimations[i], this);
        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }
    return S_OK;
}

Model* Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    Model* pInstance = new Model(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix)))
    {
        MSG_BOX("Failed to Created : Model");
        Safe_Release(pInstance);
    }
    return pInstance;
}

Component* Model::Clone(void* pArg)
{
    Model* pInstance = new Model(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Model::Free()
{
	__super::Free();

    for (auto& pAnimation : m_Animations)
        Safe_Release(pAnimation);
    m_Animations.clear();

    for (auto& pBone : m_Bones)
        Safe_Release(pBone);
    m_Bones.clear();

    for (auto& pMaterial : m_Materials)
        Safe_Release(pMaterial);
    m_Materials.clear();


    for (auto& pMesh : m_Meshes)
        Safe_Release(pMesh);

    m_Meshes.clear();

    m_Importer.FreeScene();
}
