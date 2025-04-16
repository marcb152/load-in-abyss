//
// Created by marc on 14/04/25.
//

#ifndef BOX_HPP
#define BOX_HPP

#include "bgfx/bgfx.h"
#include <memory>

namespace Abyss
{
    class BoxClass
    {
    public:
        BoxClass();
        ~BoxClass();
        
        // Initialize this box
        void init();
        
        // Set position of the box
        void setPosition(float x, float y, float z);
        
        // Set rotation of the box (in radians)
        void setRotation(float x, float y);
        
        // Get the current transform matrix
        void getTransform(float* outMatrix) const;
        
        // Render this box with the given program 
        void render(bgfx::ProgramHandle program);
        
        // Clean up resources
        void reset();
        
        // Shared initialization for all boxes
        static void initShared();
        
        // Clean up shared resources
        static void resetShared();
        
    private:
        float m_position[3] = {0.0f, 0.0f, 0.0f};
        float m_rotation[2] = {0.0f, 0.0f};
        
        // Shared resources for all boxes
        static bgfx::VertexBufferHandle ms_vbh;
        static bgfx::IndexBufferHandle ms_ibh;
        static bool ms_initialized;
        
        // Vertex structure and layout (defined in .cpp)
    };
    
    // For backward compatibility - will be deprecated in future
    namespace Box
    {
        void init();
        bgfx::VertexBufferHandle getVertexBuffer();
        bgfx::IndexBufferHandle getIndexBuffer();
        void reset();
    }
    
} // Abyss

#endif //BOX_HPP
