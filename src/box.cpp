//
// Created by marc on 14/04/25.
//

#include "box.hpp"
#include "bgfx/bgfx.h"
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
    bgfx::VertexBufferHandle BoxClass::ms_vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle BoxClass::ms_ibh = BGFX_INVALID_HANDLE;
    bool BoxClass::ms_initialized = false;

    BoxClass::BoxClass()
    {
        // Ensure shared resources are initialized
        if (!ms_initialized) {
            initShared();
        }
    }

    BoxClass::~BoxClass()
    {
        // Individual box doesn't need to clean up shared resources
    }

    void BoxClass::init()
    {
        // Initialize position and rotation
        m_position[0] = 0.0f;
        m_position[1] = 0.0f;
        m_position[2] = 0.0f;
        
        m_rotation[0] = 0.0f;
        m_rotation[1] = 0.0f;
    }

    void BoxClass::setPosition(float x, float y, float z)
    {
        m_position[0] = x;
        m_position[1] = y;
        m_position[2] = z;
    }

    void BoxClass::setRotation(float x, float y)
    {
        m_rotation[0] = x;
        m_rotation[1] = y;
    }

    void BoxClass::getTransform(float* outMatrix) const
    {
        // Create rotation matrix
        bx::mtxRotateXY(outMatrix, m_rotation[0], m_rotation[1]);
        
        // Set translation part of the matrix
        outMatrix[12] = m_position[0];
        outMatrix[13] = m_position[1];
        outMatrix[14] = m_position[2];
    }

    void BoxClass::render(bgfx::ProgramHandle program)
    {
        // Calculate and set transform
        float mtx[16];
        getTransform(mtx);
        bgfx::setTransform(mtx);

        // Set vertex and index buffer
        bgfx::setVertexBuffer(0, ms_vbh);
        bgfx::setIndexBuffer(ms_ibh);

        // Set render states
        constexpr uint64_t state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA;
        
        bgfx::setState(state);

        // Submit primitive for rendering
        bgfx::submit(0, program);
    }

    void BoxClass::reset()
    {
        // Individual box reset (not touching shared resources)
    }

    void BoxClass::initShared()
    {
        if (!ms_initialized) {
            // Create vertex stream declaration
            PosColorVertex::init();
            
            // Create static vertex buffer
            ms_vbh = bgfx::createVertexBuffer(
                    bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), 
                    PosColorVertex::ms_layout);

            // Create static index buffer
            ms_ibh = bgfx::createIndexBuffer(
                    bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
                    
            ms_initialized = true;
        }
    }

    void BoxClass::resetShared()
    {
        if (ms_initialized) {
            bgfx::destroy(ms_vbh);
            bgfx::destroy(ms_ibh);
            ms_initialized = false;
        }
    }
    
    // Legacy Box namespace for backward compatibility
    namespace Box
    {
        static bgfx::VertexBufferHandle m_vbh;
        static bgfx::IndexBufferHandle m_ibh;
        
        void init()
        {
            // Create vertex stream declaration
            PosColorVertex::init();
            
            // Create static vertex buffer
            m_vbh = bgfx::createVertexBuffer(
                    bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), 
                    PosColorVertex::ms_layout);

            // Create static index buffer
            m_ibh = bgfx::createIndexBuffer(
                    bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
        }

        bgfx::VertexBufferHandle getVertexBuffer()
        {
            return m_vbh;
        }

        bgfx::IndexBufferHandle getIndexBuffer()
        {
            return m_ibh;
        }

        void reset()
        {
            bgfx::destroy(m_vbh);
            bgfx::destroy(m_ibh);
        }
    }
} // Abyss