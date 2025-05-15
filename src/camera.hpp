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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

namespace Abyss
{

class Camera
{
public:
    Camera();
    ~Camera() = default;
    Camera(const Camera &other) = delete;
    Camera &operator=(const Camera &other) = delete;
    Camera(Camera &&other) = default;
    Camera &operator=(Camera &&other) = default;

    glm::mat4 GetMatrix() const noexcept;
    void Reset() noexcept;
    void Rotate(float dx,float dy) noexcept;
    void Translate(glm::vec3 translation) noexcept;
private:
    glm::vec3 pos;
    float pitch;
    float yaw;
    static constexpr float travelSpeed = 0.012f;
    static constexpr float rotationSpeed = 0.004f;

    template<typename T>
    T wrap_angle(T theta);
};

} // Abyss

#endif //CAMERA_HPP
