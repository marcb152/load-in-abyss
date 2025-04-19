//
// Created by marc on 19/04/25.
//

#include "imported_mesh.hpp"

#include <cassert>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "bx/math.h"

namespace Abyss
{
    ImportedMesh::ImportedMesh(const std::string& filePath)
        : m_vbh(BGFX_INVALID_HANDLE)
        , m_ibh(BGFX_INVALID_HANDLE)
        , m_initialized(false)
    {
        // Initialize transform matrix to identity
        bx::mtxIdentity(m_matrix);
        // Load the mesh from the file
        m_initialized = loadMesh(filePath);
    }

    ImportedMesh::~ImportedMesh()
    {
        reset();
    }

    void ImportedMesh::init()
    {
    }

    void ImportedMesh::render(MaterialHandle material)
    {
        if (!m_initialized || !bgfx::isValid(m_vbh) || !bgfx::isValid(m_ibh))
        {
            return;
        }
        
        // Set model transform
        bgfx::setTransform(m_matrix);
        
        // Set vertex and index buffers
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        
        // Apply material state
        bgfx::setState(material->state);
        
        // Set uniforms
        for (const auto& uniform : material->uniforms)
        {
            bgfx::setUniform(uniform.first, uniform.second);
        }
        
        // Set textures
        uint8_t textureStage = 0;
        for (const auto& texture : material->textures)
        {
            bgfx::setTexture(textureStage++, texture.first, texture.second);
        }
        
        // Submit primitive for rendering
        bgfx::submit(0, material->shader);
    }

    void ImportedMesh::reset()
    {
        if (bgfx::isValid(m_vbh))
        {
            bgfx::destroy(m_vbh);
            m_vbh = BGFX_INVALID_HANDLE;
        }
        
        if (bgfx::isValid(m_ibh))
        {
            bgfx::destroy(m_ibh);
            m_ibh = BGFX_INVALID_HANDLE;
        }

        m_initialized = false;
    }

    bool ImportedMesh::loadMesh(const std::string& filePath)
    {
        // Create an instance of the Importer class
        Assimp::Importer importer;

        // Read the model file with common post-processing options
        const aiScene* scene = importer.ReadFile(filePath,
            aiProcess_CalcTangentSpace      |
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        // Check if import failed
        if (scene == nullptr || !scene->HasMeshes())
        {
            std::cerr << "Failed to load mesh from " << filePath 
                      << ": " << importer.GetErrorString() << std::endl;
            return false;
        }

        // For now, we only load the first mesh in the scene
        const aiMesh* mesh = scene->mMeshes[0];

        // Define our vertex layout
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        // Create vertex buffer
        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(mesh->mVertices, mesh->mNumVertices * sizeof(aiVector3D)),
            layout);

        // Extract indices and convert to the appropriate format
        std::vector<uint16_t> indices;
        indices.reserve(mesh->mNumFaces * 3);
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            assert(face.mNumIndices == 3 && "Face must be triangulated");
            
            indices.push_back(static_cast<uint16_t>(face.mIndices[0]));
            indices.push_back(static_cast<uint16_t>(face.mIndices[1]));
            indices.push_back(static_cast<uint16_t>(face.mIndices[2]));
        }

        // Create index buffer
        m_ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(indices.data(), indices.size() * sizeof(uint16_t)));

        return bgfx::isValid(m_vbh) && bgfx::isValid(m_ibh);
    }
} // Abyss