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

    void ImportedMesh::render(MaterialHandle material, uint16_t viewId)
    {
        if (!m_initialized || !bgfx::isValid(m_vbh) || !bgfx::isValid(m_ibh))
        {
            return;
        }
        // Check if material is valid
        if (!material || !bgfx::isValid(material->shader))
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
        for (const auto &texture: material->textures)
        {
            bgfx::setTexture(textureStage++, texture.first, texture.second);
        }

        constexpr float lightPos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        bgfx::setUniform(u_lightPos, lightPos);
        
        // Submit primitive for rendering
        bgfx::submit(viewId, material->shader);
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
        // Destroy uniform
        bgfx::destroy(u_lightPos);

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
            aiProcess_GenNormals            |
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
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        // Creates vertex buffer
        std::vector<aiVector3D> verts_norms;
        verts_norms.reserve(mesh->mNumVertices * 2);
        for (int i = 0; i < mesh->mNumVertices; ++i)
        {
            verts_norms.push_back(mesh->mVertices[i]);
            verts_norms.push_back(mesh->mNormals[i]);
        }
        const bgfx::Memory* vbMem = bgfx::copy(verts_norms.data(), verts_norms.size() * sizeof(aiVector3D));
        m_vbh = bgfx::createVertexBuffer(vbMem, layout);

        // Extract indices and convert to the appropriate format
        std::vector<uint16_t> indices;
        // Enforce a maximum of 65,535 vertices for 16‑bit indices
        if (mesh->mNumVertices > std::numeric_limits<uint16_t>::max())
        {
            std::cerr << "Mesh too large for 16‑bit indices (" << mesh->mNumVertices
                      << " vertices). Use 32‑bit indices or split the mesh."
                      << std::endl;
            return false;
        }
        indices.reserve(mesh->mNumFaces * 3);
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            assert(face.mNumIndices == 3 && "Face must be triangulated");
            
            indices.push_back(static_cast<uint16_t>(face.mIndices[0]));
            indices.push_back(static_cast<uint16_t>(face.mIndices[1]));
            indices.push_back(static_cast<uint16_t>(face.mIndices[2]));
        }

        // Creates index buffer
        const bgfx::Memory* ibMem = bgfx::copy(indices.data(), indices.size() * sizeof(uint16_t));
        m_ibh = bgfx::createIndexBuffer(ibMem);

        // Create uniform for light position
        u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);

        importer.FreeScene();
        return bgfx::isValid(m_vbh) && bgfx::isValid(m_ibh);
    }
} // Abyss