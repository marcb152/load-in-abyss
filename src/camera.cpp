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
        const glm::vec4 forwardBaseVector = {0.0f,0.0f,1.0f,1.0f};
        // apply the camera rotations to a base vector
        glm::mat4 rotationMatrix = glm::identity<glm::mat4>();
        rotationMatrix = glm::rotate(rotationMatrix, pitch,glm::vec3(1.0f,0.0f,0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, yaw,glm::vec3(0.0f,1.0f,0.0f));
        const glm::vec4 lookVector = rotationMatrix * forwardBaseVector;
        // generate camera transform (applied to all objects to arrange them relative
        // to camera position/orientation in world) from cam position and direction
        // camera "top" always faces towards +Y (cannot do a barrel roll)
        const glm::vec3 camPosition = glm::vec3(pos);
        const glm::vec3 camTarget = camPosition + glm::vec3(lookVector);
        return glm::lookAtLH(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Camera::Reset() noexcept
    {
        pos = glm::vec3(0.0f,7.5f,-18.0f);
        pitch = 0.0f;
        yaw = 0.0f;
    }

    void Camera::Rotate( float dx,float dy ) noexcept
    {
        yaw = wrap_angle(yaw + dx * rotationSpeed);
        pitch = std::clamp(pitch + dy * rotationSpeed,0.995f * -glm::pi<float>() / 2.0f,0.995f * glm::pi<float>() / 2.0f);
    }

    void Camera::Translate( glm::vec3 translation ) noexcept
    {
        glm::mat4 rotationMatrix = glm::identity<glm::mat4>();
        rotationMatrix = glm::rotate(rotationMatrix,glm::radians(pitch),glm::vec3(1.0f,0.0f,0.0f));
        rotationMatrix = glm::rotate(rotationMatrix,glm::radians(yaw),glm::vec3(0.0f,1.0f,0.0f));
        glm::mat4 scalingMatrix = glm::identity<glm::mat4>();
        scalingMatrix = glm::scale(scalingMatrix,glm::vec3(travelSpeed));
        // TODO: Warning matrix order!!
        translation = glm::vec3(scalingMatrix * rotationMatrix * glm::vec4(translation,1.0f));
        pos = {
            pos.x + translation.x,
            pos.y + translation.y,
            pos.z + translation.z
        };
    }

    template<typename T>
    T Camera::wrap_angle( T theta )
    {
        const T modded = fmod( theta, static_cast<T>(2.0) * static_cast<T>(glm::pi<double>()) );
        return (modded > static_cast<T>(glm::pi<double>())) ?
            (modded - static_cast<T>(2.0) * static_cast<T>(glm::pi<double>())) :
            modded;
    }
} // Abyss