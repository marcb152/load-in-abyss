#ifndef MESH_HPP
#define MESH_HPP

#include "bx/bx.h"
#include "material.hpp"

namespace Abyss
{
    class Mesh
    {
    public:
        /// Constructor
        Mesh() = default;
        /// Destructor
        virtual ~Mesh() = default;
        /// Copy constructor
        Mesh(const Mesh &other) = delete;
        /// Copy assignment operator
        Mesh &operator=(const Mesh &other) = delete;
        /// Move constructor
        Mesh(Mesh &&other) = default;
        /// Move assignment operator
        Mesh &operator=(Mesh &&other) = default;

        // Core mesh functionality
        virtual void init() = 0;
        virtual void render(MaterialHandle material, int viewId) = 0;
        virtual void reset() = 0;

        // Transform methods
        virtual void setTransform(const float* matrix)
        {
            bx::memCopy(m_matrix, matrix, sizeof(float) * 16);
        }
        virtual const float* getTransform()
        {
            return m_matrix;
        }

    protected:
        float m_matrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }; // Identity matrix by default
    };
} // end namespace Abyss
#endif //MESH_HPP