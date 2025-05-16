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
/// Copyright (c) 2018 PlanetChili <http://www.planetchili.net>
/// Original source available at https://github.com/planetchili/hw3d,
/// licensed under GNU GPL v3 (https://www.gnu.org/licenses/gpl-3.0).
///
/// Contains code based on Minecraft-again experiment.
/// Copyright (c) 2020 jdah
/// Original source available at https://github.com/jdah/minecraft-again,
/// licensed under MIT (https://opensource.org/license/mit).
///
/// Modifications made by Marc for Load In Abyss.
/// This modified file, as part of Load In Abyss, is licensed under AGPLv3.
/// ==========================================================================

#include "camera.hpp"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"

namespace Abyss
{
    Camera::Camera()
    {
        Reset();
    }

    glm::mat4 Camera::GetMatrix() const noexcept
    {
        const auto direction =
            glm::normalize(
                glm::vec3(
                    glm::cos(pitch) * glm::sin(yaw),
                    -glm::sin(pitch),
                    glm::cos(pitch) * glm::cos(yaw)));
        const auto right = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
        const auto up = glm::cross(right, direction);
        const auto view =
            glm::lookAtLH(
                pos,
                pos + direction,
                up);
        return view;
    }

    void Camera::Reset() noexcept
    {
        pos = glm::vec3(0.0f,0.0f,-35.0f);
        pitch = 0.0f;
        yaw = 0.0f;
    }

    void Camera::Rotate(float dx, float dy) noexcept
    {
        pitch =
        glm::clamp(
            pitch + dy * rotationSpeed,
            -(glm::pi<float>() / 2) + glm::epsilon<float>(),
            (glm::pi<float>() / 2) - glm::epsilon<float>());
        yaw =
            (yaw < 0 ? glm::pi<float>() * 2 : 0.0f) +
                std::fmodf(yaw + dx * rotationSpeed, glm::pi<float>() * 2);
    }

    void Camera::Translate(glm::vec3 translation) noexcept
    {
        glm::mat4 rotationMatrix = glm::identity<glm::mat4>();
        rotationMatrix = glm::rotate(rotationMatrix, pitch,glm::vec3(1.0f,0.0f,0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, yaw,glm::vec3(0.0f,1.0f,0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, 0.0f ,glm::vec3(0.0f,0.0f,1.0f));
        glm::mat4 scalingMatrix = glm::identity<glm::mat4>();
        scalingMatrix = glm::scale(scalingMatrix,glm::vec3(travelSpeed));
        // Apply the rotation and scaling to the translation vector to move towards the camera's direction
        translation = glm::vec3(rotationMatrix * scalingMatrix * glm::vec4(translation,1.0f));
        pos = {
            pos.x + translation.x,
            pos.y + translation.y,
            pos.z + translation.z
        };
    }
} // Abyss