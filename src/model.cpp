/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/
/// ==========================================================================
/// Contains code based on Chili Direct3D Engine.
/// Copyright (c) PlanetChili <http://www.planetchili.net>
/// Original source available at https://github.com/planetchili/hw3d,
/// licensed under GNU GPL v3 (https://www.gnu.org/licenses/gpl-3.0).
///
/// Modifications made by Marc for Load In Abyss.
/// This modified file, as part of Load In Abyss, is licensed under AGPLv3.
/// ==========================================================================

#include "model.hpp"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "glm/gtc/type_ptr.hpp"
#include "imported_mesh.hpp"

namespace Abyss
{
    // Node
    Node::Node(std::vector<Mesh*> meshPtrs, const glm::mat4& transform)
	    :
    meshPtrs( std::move( meshPtrs ) )
    {
        this->transform = transform;
    }
    void Node::Draw(MaterialHandle material, uint16_t viewId, const glm::mat4& accumulatedTransform) const
    {
	    const auto built = transform * accumulatedTransform;
	    for (const auto pm : meshPtrs)
	    {
		    pm->render(material, viewId);
	    }
	    for( const auto& pc : childPtrs )
	    {
		    pc->Draw(material, viewId, built);
	    }
    }
    void Node::AddChild(std::unique_ptr<Node> pChild)
    {
	    assert(pChild);
	    childPtrs.push_back(std::move(pChild));
    }


    // Model
    Model::Model(const std::string & fileName)
    {
	    Assimp::Importer imp;
	    const auto pScene = imp.ReadFile( fileName.c_str(),
		    aiProcess_Triangulate |
		    aiProcess_JoinIdenticalVertices
	    );

	    for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	    {
		    meshPtrs.push_back(ParseMesh(*pScene->mMeshes[i]));
	    }

	    pRoot = ParseNode(*pScene->mRootNode);
    }
    void Model::Draw(const MaterialHandle material, const uint16_t viewId, const glm::mat4 &transform ) const
    {
	    pRoot->Draw(material, viewId, transform);
    }
    std::unique_ptr<Mesh> Model::ParseMesh(const aiMesh& mesh)
    {
        // Define our vertex layout
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;

        // Creates vertex buffer
        std::vector<aiVector3D> verts_norms;
        verts_norms.reserve(mesh.mNumVertices * 2);
        for (int i = 0; i < mesh.mNumVertices; ++i)
        {
            verts_norms.push_back(mesh.mVertices[i]);
            verts_norms.push_back(mesh.mNormals[i]);
        }
        const bgfx::Memory* vbMem = bgfx::copy(verts_norms.data(), verts_norms.size() * sizeof(aiVector3D));
        m_vbh = bgfx::createVertexBuffer(vbMem, layout);

        // Extract indices and convert to the appropriate format
        std::vector<uint16_t> indices;
        // Enforce a maximum of 65,535 vertices for 16‑bit indices
        if (mesh.mNumVertices > std::numeric_limits<uint16_t>::max())
        {
            std::cerr << "Mesh too large for 16‑bit indices (" << mesh.mNumVertices
                      << " vertices). Use 32‑bit indices or split the mesh."
                      << std::endl;
            return nullptr;
        }
        indices.reserve(mesh.mNumFaces * 3);

        for (unsigned int i = 0; i < mesh.mNumFaces; i++)
        {
            const aiFace& face = mesh.mFaces[i];
            assert(face.mNumIndices == 3 && "Face must be triangulated");

            indices.push_back(static_cast<uint16_t>(face.mIndices[0]));
            indices.push_back(static_cast<uint16_t>(face.mIndices[1]));
            indices.push_back(static_cast<uint16_t>(face.mIndices[2]));
        }

        // Creates index buffer
        const bgfx::Memory* ibMem = bgfx::copy(indices.data(), indices.size() * sizeof(uint16_t));
        m_ibh = bgfx::createIndexBuffer(ibMem);

	    return std::make_unique<ImportedMesh>("TODO");
    }
    std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
    {
	    const auto transform = glm::transpose(
	        glm::make_mat4(reinterpret_cast<const float*>(&node.mTransformation))
	        );

	    std::vector<Mesh*> curMeshPtrs;
	    curMeshPtrs.reserve(node.mNumMeshes);
	    for (size_t i = 0; i < node.mNumMeshes; i++)
	    {
		    const auto meshIdx = node.mMeshes[i];
		    curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	    }

	    auto pNode = std::make_unique<Node>(std::move(curMeshPtrs), transform);
	    for (size_t i = 0; i < node.mNumChildren; i++)
	    {
		    pNode->AddChild(ParseNode(*node.mChildren[i]));
	    }

	    return pNode;
    }
} // Abyss