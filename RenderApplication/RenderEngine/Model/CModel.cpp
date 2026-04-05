#include "CModel.h"
#include <CUtils.h>

CModel::CModel(E_MODEL_TYPE type, const S_MODEL_DESC& desc)
{
    m_desc = desc;
    switch (type) {
    case E_MODEL_FILE:
        m_ok = LoadModel(m_desc.strPath);
        break;
    case E_MODEL_CHESS:
        m_ok = InitializeChess();
        break;
    }
}

bool CModel::LoadModel(const std::string& strPath)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(strPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        PRINTLOG("ERROR::ASSIMP:: %s", importer.GetErrorString());
        return false;
    }
    size_t nPos = strPath.find_last_of("/\\");
    if (strPath.npos != nPos) {
        m_strRootPath = strPath.substr(0, nPos);
    }
    // process ASSIMP's root node recursively
    return ProcessNode(scene->mRootNode, scene);
}

bool CModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* pMesh = scene->mMeshes[node->mMeshes[i]];
        m_vec_mesh.push_back(ProcessMesh(pMesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        if (!ProcessNode(node->mChildren[i], scene)) {
            return false;
        }
    }
    return true;
}

CMesh CModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<CMesh::S_VERTEX>    vertices;
    std::vector<unsigned int>       indices;
    std::vector<CMesh::S_TEXTURE>   textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        CMesh::S_VERTEX vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 1. diffuse maps
    std::vector<CMesh::S_TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<CMesh::S_TEXTURE> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<CMesh::S_TEXTURE> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<CMesh::S_TEXTURE> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return CMesh(vertices, indices, textures);
}

std::vector<CMesh::S_TEXTURE> CModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<CMesh::S_TEXTURE> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        if (aiReturn_SUCCESS == mat->GetTexture(type, i, &str)) {
            CMesh::S_TEXTURE texture;
            CTexture::S_TEXTURE_DESC desc;
            desc.strPath = (m_strRootPath + "/" + str.C_Str());
            texture.texture = CResourceManager::AquireTexture(desc);
            if (nullptr == texture.texture) {
                PRINTLOG("Fail to load texture(%s)", desc.strPath.c_str());
                continue;
            }
            texture.strType = typeName;
            texture.strPath = str.C_Str();
            textures.push_back(texture);
        }
    }
    return textures;
}

bool CModel::InitializeRectangle(float size)
{
    std::vector<CMesh::S_VERTEX>	vertices;
    std::vector<unsigned int>	indices = {
        0, 1, 2,
        2, 3, 0
    };
    CMesh::S_VERTEX	LeftTop, RightTop, RightBottom, LeftBottom;
    LeftTop.Position = glm::vec3(-1.0f, 0.0f, -1.0f);
    LeftTop.TexCoords = glm::vec2(0.0f, size);

    RightTop.Position = glm::vec3(1.0f, 0.0f, -1.0f);
    RightTop.TexCoords = glm::vec2(size, size);

    RightBottom.Position = glm::vec3(1.0f, 0.0f, 1.0f);
    RightBottom.TexCoords = glm::vec2(size, 0.0f);

    LeftBottom.Position = glm::vec3(-1.0f, 0.0f, 1.0f);
    LeftBottom.TexCoords = glm::vec2(0.0f, 0.0f);

    vertices.push_back(LeftTop);
    vertices.push_back(RightTop);
    vertices.push_back(RightBottom);
    vertices.push_back(LeftBottom);

    m_vec_mesh.push_back(CMesh(vertices, indices));
    return true;
}

bool CModel::InitializeChess()
{
    if (!InitializeRectangle(m_desc.size)) {
        return false;
    }
    std::vector<CMesh::S_TEXTURE>	textures;

    CMesh::S_TEXTURE texture;
    texture.strPath = CUtils::GetImagePathFile("chess.jpg");
    texture.strType = "texture_diffuse";

    CTexture::S_TEXTURE_DESC desc;
    desc.strPath = texture.strPath;
    texture.texture = CResourceManager::AquireTexture(desc);
    if (0 == texture.texture) {
        PRINTLOG("Fail to create chess texture");
        return false;
    }
    textures.push_back(texture);
    m_vec_mesh[0].m_vec_Textures = textures;
    return true;
}