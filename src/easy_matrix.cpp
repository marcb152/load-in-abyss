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

    EasyMatrix& EasyMatrix::scale(float s)
    {
        return scale(s, s, s);
    }

    EasyMatrix& EasyMatrix::scale(float sx, float sy, float sz)
    {
        float temp[16] = {};
        bx::mtxScale(temp, sx, sy, sz);
        return combine(temp);
    }

    EasyMatrix& EasyMatrix::rotateXY(float ax, float ay)
    {
        float temp[16] = {};
        bx::mtxRotateXY(temp, ax, ay);
        return combine(temp);
    }

    EasyMatrix& EasyMatrix::translate(float x, float y, float z)
    {
        float temp[16] = {};
        bx::mtxTranslate(temp, x, y, z);
        return combine(temp);
    }

    const float* EasyMatrix::getMatrix() const
    {
        return m_matrix;
    }

    EasyMatrix& EasyMatrix::combine(const float* temp)
    {
        float copy[16] = {};
        bx::memCopy(copy, m_matrix, sizeof(float) * 16);
        bx::mtxMul(m_matrix, copy, temp);
        return *this;
    }
} // Abyss