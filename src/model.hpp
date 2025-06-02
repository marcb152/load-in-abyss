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

#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include <vector>

#include "assimp/scene.h"
#include "glm/gtc/matrix_transform.hpp"
#include "mesh.hpp"

namespace Abyss
{
    class Node
    {
        friend class Model;
    public:
        Node(std::vector<Mesh *> meshPtrs, const glm::mat4& transform);
        void Draw(MaterialHandle material, uint16_t viewId, const glm::mat4& accumulatedTransform) const;
    private:
        void AddChild(std::unique_ptr<Node> pChild);
    private:
        std::vector<std::unique_ptr<Node>> childPtrs;
        std::vector<Mesh*> meshPtrs;
        glm::mat4 transform;
    };

    class Model
    {
    public:
        Model(const std::string & fileName);
        void Draw(MaterialHandle material, uint16_t viewId, const glm::mat4 &transform) const;
    private:
        static std::unique_ptr<Mesh> ParseMesh(const aiMesh& mesh);
        std::unique_ptr<Node> ParseNode(const aiNode& node);
    private:
        std::unique_ptr<Node> pRoot;
        std::vector<std::unique_ptr<Mesh>> meshPtrs;
    };
} // Abyss

#endif //MODEL_HPP
