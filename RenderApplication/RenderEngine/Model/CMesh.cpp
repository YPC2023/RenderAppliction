#include "CMesh.h"
#include <glad/glad.h>


CMesh::CMesh(std::vector<S_VERTEX> vertices,
    std::vector<unsigned int> indices,
    float vertexResize,
    float textureResize,
    unsigned int type)
{
    m_nType = type;
    m_vec_Vertices = vertices;
    m_vec_Indices = indices;
    m_VertexResize = vertexResize;
    m_TextureResize = textureResize;
    Resize();
    SetupMesh();
}

CMesh::CMesh(std::vector<S_VERTEX> vertices,
    std::vector<unsigned int> indices,
    std::vector<S_TEXTURE> textures,
    float vertexResize,
    float textureResize,
    unsigned int type) :CMesh(vertices, indices, vertexResize, textureResize, type)
{
    this->m_vec_Textures = textures;
}

CMesh::~CMesh()
{
}

void CMesh::Resize()
{
    // 顶点&坐标放大
    for (size_t index = 0; index < m_vec_Vertices.size(); ++index)
    {
        m_vec_Vertices[index].Position.m_value *= m_VertexResize;
        m_vec_Vertices[index].TexCoords.m_value *= m_TextureResize;
    }
}

void CMesh::SetupMesh()
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