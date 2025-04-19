//
// Created by marc on 14/04/25.
//

#ifndef BOX_HPP
#define BOX_HPP

#include "bgfx/bgfx.h"

#include "mesh.hpp"
#include "material.hpp"

namespace Abyss
{
    class Box final : Mesh
    {
    public:
        /// Constructor
        Box();
        /// Destructor
        ~Box() override = default;
        /// Copy constructor
        Box(const Box &other) = delete;
        /// Copy assignment operator
        Box &operator=(const Box &other) = delete;
        /// Move constructor
        Box(Box &&other) = default;
        /// Move assignment operator
        Box &operator=(Box &&other) = default;

        // Initialize this box with identity matrix
        void init() override;
        
        // Set the transform matrix directly
        void setTransform(const float* matrix) override;

        // Render this box with the given program 
        void render(MaterialHandle material) override;
        
        // Clean up resources
        void reset() override;
        
        // Shared initialization for all boxes
        static void initShared();
        
        // Clean up shared resources
        static void resetShared();
        
    private:
        static bgfx::VertexBufferHandle ms_vbh;
        static bgfx::IndexBufferHandle ms_ibh;
        static bool ms_initialized;
    };
    
} // Abyss

#endif //BOX_HPP
