//
// Created by marc on 14/04/25.
//

#ifndef BOX_HPP
#define BOX_HPP

#include "bgfx/bgfx.h"

#include "model.hpp"

namespace Abyss
{
    class Box
    {
    public:
        /// Constructor
        Box();
        /// Destructor
        ~Box() = default;
        /// Copy constructor
        Box(const Box &other) = delete;
        /// Copy assignment operator
        Box &operator=(const Box &other) = delete;
        /// Move constructor
        Box(Box &&other) = default;
        /// Move assignment operator
        Box &operator=(Box &&other) = default;

        // Initialize this box with identity matrix
        void init();
        
        // Set the transform matrix directly
        void setMatrix(const float* matrix);
        
        // Get the transform matrix directly
        const float* getMatrix() const;
        
        // Render this box with the given program 
        void render(bgfx::ProgramHandle program);
        
        // Clean up resources
        void reset();
        
        // Shared initialization for all boxes
        static void initShared();
        
        // Clean up shared resources
        static void resetShared();
        
    private:
        float m_matrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }; // Identity matrix by default

        // Shared resources for all boxes
        static bgfx::VertexBufferHandle ms_vbh;
        static bgfx::IndexBufferHandle ms_ibh;
        static bool ms_initialized;
    };
    
} // Abyss

#endif //BOX_HPP
