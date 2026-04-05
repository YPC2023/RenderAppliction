#include "CFileModel.h"
#include <CUtils.h>

CFileModel::CFileMesh::CFileMesh(std::vector<S_VERTEX> vertices,
    std::vector<unsigned int> indices,
    std::vector<S_TEXTURE> textures,
    unsigned int type)
{
    m_vec_Vertices = vertices;
    m_vec_Indices = indices;
    m_vec_Textures = textures;
    m_nType = type;
    SetupMesh();
}

void CFileModel::CFileMesh::SetupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, m_vec_Vertices.size() * sizeof(S_VERTEX), &m_vec_Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vec_Indices.size() * sizeof(unsigned int), &m_vec_Indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Color));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Bitangent));

    glEnableVertexAttribArray(0);
}

CFileModel::CFileModel(const std::string& strPath)
{
    m_ok = LoadModel(strPath);
    PRINTLOG("YPC");
}

bool CFileModel::LoadModel(const std::string& strPath)
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

bool CFileModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* pMesh = scene->mMeshes[node->mMeshes[i]];
        CFileMesh mesh = ProcessMesh(pMesh, scene);
        m_vec_Mesh.push_back(mesh);
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

CFileModel::CFileMesh CFileModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<CFileMesh::S_VERTEX>    vertices;
    std::vector<unsigned int>           indices;
    std::vector<CFileMesh::S_TEXTURE>   textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        CFileMesh::S_VERTEX vertex;
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
    std::vector<CFileMesh::S_TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<CFileMesh::S_TEXTURE> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<CFileMesh::S_TEXTURE> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<CFileMesh::S_TEXTURE> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return CFileMesh(vertices, indices, textures);
}

std::vector<CFileModel::CFileMesh::S_TEXTURE> CFileModel::LoadMaterialTextures(
    aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<CFileMesh::S_TEXTURE> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        if (aiReturn_SUCCESS == mat->GetTexture(type, i, &str)) {
            CFileMesh::S_TEXTURE texture;
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