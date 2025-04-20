//
// Created by marc on 20/04/25.
//

#include "easy_matrix.hpp"

#include "bx/math.h"

namespace Abyss
{
    EasyMatrix::EasyMatrix()
    {
        bx::mtxIdentity(m_matrix);
    }

    EasyMatrix& EasyMatrix::Scale(float scale)
    {
        return Scale(scale, scale, scale);
    }

    EasyMatrix& EasyMatrix::Scale(float sx, float sy, float sz)
    {
        float temp[16] = {};
        bx::mtxScale(temp, sx, sy, sz);
        return Combine(temp);
    }

    EasyMatrix& EasyMatrix::RotateXY(float ax, float ay)
    {
        float temp[16] = {};
        bx::mtxRotateXY(temp, ax, ay);
        return Combine(temp);
    }

    EasyMatrix& EasyMatrix::Translate(float x, float y, float z)
    {
        float temp[16] = {};
        bx::mtxTranslate(temp, x, y, z);
        return Combine(temp);
    }

    const float* EasyMatrix::GetMatrix() const
    {
        return m_matrix;
    }

    EasyMatrix& EasyMatrix::Combine(const float* temp)
    {
        float copy[16] = {};
        bx::memCopy(copy, m_matrix, sizeof(float) * 16);
        bx::mtxMul(m_matrix, copy, temp);
        return *this;
    }
} // Abyss