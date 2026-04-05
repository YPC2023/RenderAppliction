#include "CModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    case E_MODEL_COLUMN:
        m_ok = InitializeColumn();
        break;
    case E_MODEL_SPHERE:
        m_ok = InitializeSphere();
        break;
    case E_MODEL_CONE:
        m_ok = InitializeCone();
        break;
    case E_MODEL_TORUS:
        m_ok = InitializeTorus();
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

bool CModel::InitializeRectangle()
{
    std::vector<CMesh::S_VERTEX>	vertices;
    std::vector<unsigned int>	indices = {
        0, 1, 2,
        2, 3, 0
    };
    CMesh::S_VERTEX	LeftTop, RightTop, RightBottom, LeftBottom;
    LeftTop.Position = glm::vec3(-1.0f, 0.0f, -1.0f);
    LeftTop.TexCoords = glm::vec2(0.0f, 1.0f);

    RightTop.Position = glm::vec3(1.0f, 0.0f, -1.0f);
    RightTop.TexCoords = glm::vec2(1.0f, 1.0f);

    RightBottom.Position = glm::vec3(1.0f, 0.0f, 1.0f);
    RightBottom.TexCoords = glm::vec2(1.0f, 0.0f);

    LeftBottom.Position = glm::vec3(-1.0f, 0.0f, 1.0f);
    LeftBottom.TexCoords = glm::vec2(0.0f, 0.0f);

    vertices.push_back(LeftTop);
    vertices.push_back(RightTop);
    vertices.push_back(RightBottom);
    vertices.push_back(LeftBottom);

    m_vec_mesh.push_back(CMesh(vertices, indices, m_desc.vertexResize, m_desc.textureResize));
    return true;
}

bool CModel::InitializeChess()
{
    if (!InitializeRectangle()) {
        return false;
    }
    std::vector<CMesh::S_TEXTURE>	textures;

    CMesh::S_TEXTURE texture;
    texture.strPath = CUtils::GetImagePathFile("chess.jpg");
    texture.strType = "texture_diffuse";

    CTexture::S_TEXTURE_DESC desc;
    desc.strPath = texture.strPath;
    desc.TEX_GL_TEXTURE_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR;
    texture.texture = CResourceManager::AquireTexture(desc);
    if (0 == texture.texture) {
        PRINTLOG("Fail to create chess texture");
        return false;
    }
    textures.push_back(texture);
    m_vec_mesh[0].m_vec_Textures = textures;
    return true;
}

bool CModel::InitializeColumn()
{
    std::vector<CMesh::S_VERTEX>	vertices;
    std::vector<CMesh::S_TEXTURE>	textures;
    std::vector<unsigned int>	indices;
    GenerateColumnVertex(m_desc.S_MODEL_COLUMN_DESC.start, 
        m_desc.S_MODEL_COLUMN_DESC.end, 
        vertices, indices, 
        m_desc.S_MODEL_COLUMN_DESC.radius, 
        m_desc.S_MODEL_COLUMN_DESC.sectors);
    m_vec_mesh.push_back(CMesh(vertices, indices, m_desc.vertexResize, m_desc.textureResize));
    return true;
}

bool CModel::InitializeSphere()
{
    std::vector<CMesh::S_VERTEX>	vertices;
    std::vector<CMesh::S_TEXTURE>	textures;
    std::vector<unsigned int>	indices;
    GenerateSphereVertex(m_desc.S_MODEL_SPHERE_DESC.center, 
        m_desc.S_MODEL_SPHERE_DESC.radius, 
        m_desc.S_MODEL_SPHERE_DESC.sectors, 
        m_desc.S_MODEL_SPHERE_DESC.stacks, 
        m_desc.S_MODEL_SPHERE_DESC.color, 
        vertices, indices);
    m_vec_mesh.push_back(CMesh(vertices, indices, m_desc.vertexResize, m_desc.textureResize));
    return true;
}

bool CModel::InitializeCone()
{
    std::vector<CMesh::S_VERTEX>	vertices;
    std::vector<CMesh::S_TEXTURE>	textures;
    std::vector<unsigned int>	indices;
    GenerateConeVertex(m_desc.S_MODEL_CONE_DESC.center, 
        m_desc.S_MODEL_CONE_DESC.axisDir, 
        m_desc.S_MODEL_CONE_DESC.height, 
        m_desc.S_MODEL_CONE_DESC.radius, 
        m_desc.S_MODEL_CONE_DESC.sectors, 
        m_desc.S_MODEL_CONE_DESC.color, 
        vertices, indices);
    m_vec_mesh.push_back(CMesh(vertices, indices, m_desc.vertexResize, m_desc.textureResize));
    return true;
}

bool CModel::InitializeTorus()
{
    std::vector<CMesh::S_VERTEX>	vertices;
    std::vector<unsigned int>	indices;
    GenerateTorusVertex(m_desc.S_MODEL_TORUS_DESC.center, 
        m_desc.S_MODEL_TORUS_DESC.axisDir, 
        m_desc.S_MODEL_TORUS_DESC.color, 
        m_desc.S_MODEL_TORUS_DESC.radius, 
        m_desc.S_MODEL_TORUS_DESC.width, 
        m_desc.S_MODEL_TORUS_DESC.mainSectors, 
        m_desc.S_MODEL_TORUS_DESC.tubeSectors, 
        vertices, indices);

    m_vec_mesh.push_back(CMesh(vertices, indices, m_desc.vertexResize, m_desc.textureResize));
    return true;
}

void CModel::GenerateTorusVertex(const glm::vec3& center, const glm::vec3& axisNormal, const glm::vec4& color,
    float outerRadius, float ringWidth, int mainSectors, int tubeSectors,
    std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices) {

    vertices.clear();
    indices.clear();

    // 1. 参数计算
    float r = ringWidth / 2.0f;          // 环管半径
    float R = outerRadius - r;           // 主圆半径 (圆心到管中心的距离)

    // 2. 精确构建平面的正交基 (Orthonormal Basis)
    glm::vec3 n = glm::normalize(axisNormal); // 圆环平面的法线（转轴）

    // 寻找一个不与法线平行的辅助向量
    glm::vec3 helper = (glm::abs(n.y) < 0.999f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);

    // u 和 w 构成了圆环所在的平面
    glm::vec3 u = glm::normalize(glm::cross(helper, n));
    glm::vec3 w = glm::normalize(glm::cross(n, u));

    // 3. 生成顶点
    for (int i = 0; i <= mainSectors; ++i) {
        // vAngle 控制绕圆环大圈旋转
        float vAngle = (float)i * 2.0f * glm::pi<float>() / (float)mainSectors;

        // currentCycleDir 是在大圆平面上的径向向量
        glm::vec3 currentCycleDir = u * cosf(vAngle) + w * sinf(vAngle);

        for (int j = 0; j <= tubeSectors; ++j) {
            // uAngle 控制绕环管小圈旋转
            float uAngle = (float)j * 2.0f * glm::pi<float>() / (float)tubeSectors;

            // tubeOffset: 
            // cos(uAngle) 部分沿大圆径向（currentCycleDir）
            // sin(uAngle) 部分沿平面法向（n）
            glm::vec3 tubeOffset = (currentCycleDir * cosf(uAngle) + n * sinf(uAngle)) * r;

            // 环管中心点坐标
            glm::vec3 tubeCenter = center + currentCycleDir * R;

            CMesh::S_VERTEX vertex;
            vertex.Position = tubeCenter + tubeOffset;
            vertex.Normal = glm::normalize(tubeOffset); // 法线从管心指向表面
            vertex.Color = color;

            vertices.push_back(vertex);
        }
    }

    // 4. 生成索引 (与之前一致)
    for (int i = 0; i < mainSectors; ++i) {
        for (int j = 0; j < tubeSectors; ++j) {
            int currStack = i * (tubeSectors + 1);
            int nextStack = (i + 1) * (tubeSectors + 1);

            int b1 = currStack + j;
            int b2 = nextStack + j;
            int b3 = nextStack + j + 1;
            int b4 = currStack + j + 1;

            indices.push_back(b1); indices.push_back(b2); indices.push_back(b3);
            indices.push_back(b1); indices.push_back(b3); indices.push_back(b4);
        }
    }
}

void CModel::GenerateConeVertex(const glm::vec3& center, const glm::vec3& axisDir,
    float height, float radius, int sectors, const glm::vec4& color,
    std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices) {

    // 1. 清空原始数据
    vertices.clear();
    indices.clear();

    // 2. 基础向量：确定主轴和锥尖
    glm::vec3 normAxis = glm::normalize(axisDir);
    glm::vec3 tip = center + normAxis * height;   // 锥尖位置 = 底面中心 + 方向 * 高度

    // 3. 构建底面坐标系 (正交基)
    // 寻找一个不与主轴平行的向量来做叉乘
    glm::vec3 up = (glm::abs(normAxis.y) < 0.999f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
    glm::vec3 u = glm::normalize(glm::cross(up, normAxis));
    glm::vec3 w = glm::cross(normAxis, u);

    // --- A. 生成侧面 (Side Surface) ---
    // 顶点：锥尖 (Index 0)
    int tipIndex = (int)vertices.size();
    CMesh::S_VERTEX vTip;
    vTip.Position = tip;
    vTip.Normal = normAxis; // 侧面顶点的法线暂设为主轴方向
    vTip.Color = color;
    vertices.push_back(vTip);

    // 顶点：侧面圆周点 (从 Index 1 到 sectors+1)
    int sideCircleStart = (int)vertices.size();
    for (int i = 0; i <= sectors; ++i) {
        float angle = (float)i * 2.0f * glm::pi<float>() / (float)sectors;
        glm::vec3 posOnCircle = center + (u * cosf(angle) + w * sinf(angle)) * radius;

        CMesh::S_VERTEX v;
        v.Position = posOnCircle;
        v.Color = color;

        // 计算平滑的侧面法线
        glm::vec3 toTip = glm::normalize(tip - posOnCircle);
        glm::vec3 tangent = glm::cross(normAxis, posOnCircle - center);
        v.Normal = glm::normalize(glm::cross(tangent, toTip));

        vertices.push_back(v);
    }

    // --- B. 生成底面 (Base Disk) ---
    // 顶点：底面中心 (Index baseCenterIndex)
    int baseCenterIndex = (int)vertices.size();
    CMesh::S_VERTEX vBaseCenter;
    vBaseCenter.Position = center;
    vBaseCenter.Normal = -normAxis; // 底面法线朝向锥尖的反方向
    vBaseCenter.Color = color;
    vertices.push_back(vBaseCenter);

    // 顶点：底面圆周点 (独立的顶点以保证法线垂直)
    int baseCircleStart = (int)vertices.size();
    for (int i = 0; i <= sectors; ++i) {
        float angle = (float)i * 2.0f * glm::pi<float>() / (float)sectors;
        CMesh::S_VERTEX v;
        v.Position = center + (u * cosf(angle) + w * sinf(angle)) * radius;
        v.Normal = -normAxis;
        v.Color = color;
        vertices.push_back(v);
    }

    // --- C. 索引 (Indices) ---
    // 侧面索引：连接锥尖和侧面圆周
    for (int i = 0; i < sectors; ++i) {
        indices.push_back(tipIndex);                   // 锥尖
        indices.push_back(sideCircleStart + i);         // 当前圆周点
        indices.push_back(sideCircleStart + i + 1);     // 下一个圆周点
    }

    // 底面索引：连接底心和底面圆周
    for (int i = 0; i < sectors; ++i) {
        indices.push_back(baseCenterIndex);            // 底心
        indices.push_back(baseCircleStart + i + 1);     // 下一个圆周点
        indices.push_back(baseCircleStart + i);         // 当前圆周点 (注意顺序，反向以保持顺时针/逆时针一致)
    }
}

// 参数：起点，终点，半径，圆周分段数
void CModel::GenerateColumnVertex(glm::vec3 start, glm::vec3 end,
    std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices,
    float radius, int sectors, const glm::vec4& sColor, const glm::vec4& eColor)
{
    CMesh::S_VERTEX vertex;
    glm::vec3 dir = end - start;
    float height = glm::length(dir);
    glm::vec3 L = glm::normalize(dir);

    // 1. 构建正交基 U 和 V
    glm::vec3 worldUp = (std::abs(L.y) > 0.999f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
    glm::vec3 U = glm::normalize(glm::cross(worldUp, L));
    glm::vec3 V = glm::cross(L, U);

    // 清空原始数据
    vertices.clear();
    indices.clear();

    // --- 步骤 A: 生成侧面顶点 ---
    // 侧面需要两圈顶点（底部和顶部）
    for (int i = 0; i <= sectors; ++i) {
        float angle = (float)i / sectors * glm::two_pi<float>();
        glm::vec3 offset = (cos(angle) * U + sin(angle) * V);

        // 侧面法线就是 offset 向量本身
        glm::vec3 norm = glm::normalize(offset);

        // 底部圆周顶点
        vertex.Position = start + radius * offset;
        vertex.Normal = norm;
        vertex.Color = sColor;
        vertices.push_back(vertex);
        // 顶部圆周顶点
        vertex.Position = end + radius * offset;
        vertex.Normal = norm;
        vertex.Color = eColor;
        vertices.push_back(vertex);
    }

    // 侧面索引 (Triangle Strip 逻辑)
    for (int i = 0; i < sectors; ++i) {
        int b1 = i * 2;
        int t1 = i * 2 + 1;
        int b2 = (i + 1) * 2;
        int t2 = (i + 1) * 2 + 1;
        // 第一个三角形
        indices.push_back(b1); indices.push_back(t1); indices.push_back(b2);
        // 第二个三角形
        indices.push_back(t1); indices.push_back(t2); indices.push_back(b2);
    }

    // --- 步骤 B: 生成底面和顶面 (盖子) ---
    unsigned int baseCenterIdx = (unsigned int)vertices.size();
    vertex.Position = start;
    vertex.Normal = -L;
    vertex.Color = sColor;
    vertices.push_back(vertex); // 底中心，法线向下
    unsigned int topCenterIdx = (unsigned int)vertices.size();
    vertex.Position = end;
    vertex.Normal = L;
    vertex.Color = eColor;
    vertices.push_back(vertex);    // 顶中心，法线向上

    for (int i = 0; i <= sectors; ++i) {
        float angle = (float)i / sectors * glm::two_pi<float>();
        glm::vec3 offset = (cos(angle) * U + sin(angle) * V);

        unsigned int currBase = (unsigned int)vertices.size();
        vertex.Position = start + radius * offset;
        vertex.Normal = -L;
        vertex.Color = sColor;
        vertices.push_back(vertex);
        unsigned int currTop = (unsigned int)vertices.size();
        vertex.Position = end + radius * offset;
        vertex.Normal = L;
        vertex.Color = eColor;
        vertices.push_back(vertex);

        if (i < sectors) {
            // 底面索引
            indices.push_back(baseCenterIdx);
            indices.push_back(currBase + 2);
            indices.push_back(currBase);

            // 顶面索引
            indices.push_back(topCenterIdx);
            indices.push_back(currTop);
            indices.push_back(currTop + 2);
        }
    }
}

void CModel::GenerateSphereVertex(const glm::vec3& center, float radius, int sectors, int stacks,
    const glm::vec4& color, std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices)
{

    // 清除旧数据，防止重复累加
    vertices.clear();
    indices.clear();

    float sectorStep = 2.0f * glm::pi<float>() / sectors;
    float stackStep = glm::pi<float>() / stacks;

    for (int i = 0; i <= stacks; ++i) {
        // 从北极 (pi/2) 扫描到 南极 (-pi/2)
        float stackAngle = glm::pi<float>() / 2.0f - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;

            // 1. 创建顶点实例
            CMesh::S_VERTEX vertex;

            // 2. 计算位置 (相对于原点计算后，加上传入的 center)
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            vertex.Position = glm::vec3(x, y, z) + center;

            // 3. 计算法线 (法线不随位置平移，只跟相对于球心的方向有关)
            // 归一化 (x, y, z) 即为该点的法线
            vertex.Normal = glm::normalize(glm::vec3(x, y, z));

            // 4. 赋值颜色
            vertex.Color = color;

            // 5. 如果你的结构体还有 UV 坐标成员，可以在这里补充
            // vertex.TexCoords = glm::vec2((float)j / sectors, (float)i / stacks);

            vertices.push_back(vertex);
        }
    }

    // --- 生成索引 (Indices) ---
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);     // 当前层起点
        int k2 = k1 + sectors + 1;      // 下一层起点

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // 每四个点组成两个三角形
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}