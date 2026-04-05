#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Resource/CResourceManager.h"
#include "../Core/Define.h"

class CFileModel
{
private:
	class CFileMesh
	{
    public:
        class Vec2
        {
            friend class CMesh;
        public:
            Vec2()
            {
                m_enable = false;
                m_value = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        public:
            Vec2 operator = (glm::vec2 v)
            {
                m_enable = true;
                m_value = v;
                return *this;
            }
        private:
            glm::vec2   m_value;
        protected:
            bool        m_enable;
        };

        class Vec3
        {
            friend class CMesh;
        public:
            Vec3()
            {
                m_enable = false;
                m_value = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        public:
            Vec3 operator = (glm::vec3 v)
            {
                m_enable = true;
                m_value = v;
                return *this;
            }
        private:
            glm::vec3   m_value;
        protected:
            bool        m_enable;
        };

        class Vec4
        {
            friend class CMesh;
        public:
            Vec4()
            {
                m_enable = false;
                m_value = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            }
        public:
            Vec4 operator = (glm::vec4 v)
            {
                m_enable = true;
                m_value = v;
                return *this;
            }
        private:
            glm::vec4   m_value;
        protected:
            bool        m_enable;
        };

    public:
        typedef struct _S_VERTEX {
            // position
            Vec3 Position;
            // Color
            Vec4 Color;
            // normal
            Vec3 Normal;
            // texCoords
            Vec2 TexCoords;
            // tangent
            Vec3 Tangent;
            // bitangent
            Vec3 Bitangent;
        }S_VERTEX;
        typedef struct _S_TEXTURE {
            std::shared_ptr<CTexture>   texture;
            std::string                 strType;
            std::string                 strPath;
        }S_TEXTURE;
    public:
        CFileMesh(std::vector<S_VERTEX> vertices,
            std::vector<unsigned int> indices,
            std::vector<S_TEXTURE> textures,
            unsigned int type = GL_TRIANGLES);
    private:
        void SetupMesh();

    public:
        std::vector<S_VERTEX>       m_vec_Vertices;
        std::vector<unsigned int>   m_vec_Indices;
        std::vector<S_TEXTURE>      m_vec_Textures;
    public:
        unsigned int                m_nType;
        HVAO                        m_VAO;
        HVBO                        m_VBO;
        HEBO                        m_EBO;
	};
public:
    CFileModel(const std::string& strPath);
private:
	bool LoadModel(const std::string& path);
	bool ProcessNode(aiNode* node, const aiScene* scene);
    CFileMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<CFileMesh::S_TEXTURE> LoadMaterialTextures(aiMaterial* mat,
        aiTextureType type, std::string typeName);
public:
	bool					    m_ok;
	std::string				    m_strRootPath;
public:
	std::string				    m_strPath;
	std::vector<CFileMesh>	    m_vec_Mesh;
};