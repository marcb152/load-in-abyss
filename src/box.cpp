//
// Created by marc on 14/04/25.
//

#include "box.hpp"

#include <iostream>
#include <memory>

#include "bx/math.h"

namespace Abyss
{
    struct PosColorVertex
    {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_abgr;

        static void init()
        {
            ms_layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
                .end();
        };

        static bgfx::VertexLayout ms_layout;
    };

    bgfx::VertexLayout PosColorVertex::ms_layout;

    static PosColorVertex s_cubeVertices[] =
    {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };

    static const uint16_t s_cubeTriList[] =
    {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };

    // Static members of Box class
    bgfx::VertexBufferHandle Box::ms_vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle Box::ms_ibh = BGFX_INVALID_HANDLE;
    bool Box::ms_initialized = false;

    Box::Box() : Mesh()
    {
        // Ensure shared resources are initialized
        if (!ms_initialized)
        {
            initShared();
        }
    }

    void Box::init()
    {
        // Initialize with identity matrix
        bx::mtxIdentity(m_matrix);
    }
    
    void Box::setTransform(const float* matrix)
    {
        bx::memCopy(m_matrix, matrix, sizeof(float) * 16);
    }

    void Box::render(MaterialHandle material)
    {
        // Set transform directly from the matrix
        bgfx::setTransform(m_matrix);

        // Set vertex and index buffer
        bgfx::setVertexBuffer(0, ms_vbh);
        bgfx::setIndexBuffer(ms_ibh);
        
        bgfx::setState(material->state);

        // Submit primitive for rendering
        bgfx::submit(0, material->shader);
    }

    void Box::reset()
    {
        // Individual box reset (not touching shared resources)
    }

    void Box::initShared()
    {
        if (!ms_initialized)
        {
            // Create vertex stream declaration
            PosColorVertex::init();
            
            // Create static vertex buffer
            ms_vbh = bgfx::createVertexBuffer(
                    bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), 
                    PosColorVertex::ms_layout);

            // Create static index buffer
            ms_ibh = bgfx::createIndexBuffer(
                    bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

            // Check if handles are valid after creation
            if (bgfx::isValid(ms_vbh) && bgfx::isValid(ms_ibh))
            {
                ms_initialized = true;
            }
            else
            {
                // Handle error: Failed to create buffers
                std::cerr << "Failed to create vertex/index buffers" << std::endl;
            }

        }
    }

    void Box::resetShared()
    {
        if (ms_initialized)
        {
            bgfx::destroy(ms_vbh);
            bgfx::destroy(ms_ibh);
            ms_initialized = false;
        }
    }
} // Abyss