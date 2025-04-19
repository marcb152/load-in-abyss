//
// Created by marc on 14/04/25.
//

#include "renderer.hpp"

#include <cassert>

#include "box.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h> // Output data structure
#include <memory>
#include <vector>
#include "bgfx/platform.h"
#include "bx/readerwriter.h"
#include "bx/timer.h"

namespace Abyss::renderer
{
    bgfx::ProgramHandle m_simple_program;
    int64_t m_timeOffset;

    // Scene management
    std::vector<std::shared_ptr<Box>> m_boxes;

    static bgfx::ShaderHandle loadShader(const char *FILENAME);
    const bgfx::ViewId kClearView = 0;

    // Static members of assimp suzanne loading
    bgfx::VertexBufferHandle m_vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle m_ibh = BGFX_INVALID_HANDLE;

    Material material = {};

    int init(bgfx::Init init)
    {
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();

        if (!bgfx::init(init))
            return 1;
        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
        // bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

        // Initialize shared box resources
        Box::initShared();
        
        // Create program from shaders.
        // load all programs in shaders directory
        bgfx::ShaderHandle vsh = loadShader("vs_cubes.sc.bin");
        bgfx::ShaderHandle fsh = loadShader("fs_cubes.sc.bin");
        material.shader = bgfx::createProgram(vsh, fsh, true);
        material.state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA;

        vsh = loadShader("vs_simple.sc.bin");
        fsh = loadShader("fs_simple.sc.bin");
        m_simple_program = bgfx::createProgram(vsh, fsh, true);

        // Assimp loading for suzanne (temporary, for testing purposes)
        {
            // TODO:
            // Create an instance of the Importer class
            Assimp::Importer importer;

            // And have it read the given file with some example postprocessing
            // Usually - if speed is not the most important aspect for you - you'll
            // probably to request more postprocessing than we do in this example.
            const aiScene* scene = importer.ReadFile( "models/suzanne.obj",
              aiProcess_CalcTangentSpace |
              aiProcess_Triangulate            |
              aiProcess_JoinIdenticalVertices  |
              aiProcess_SortByPType);

            // If the import failed, report it
            if (nullptr == scene) {
                std::cerr << importer.GetErrorString() << std::endl;
                return false;
            }

            bgfx::VertexLayout ms_layout;
            ms_layout
                    .begin()
                    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                    .end();

            m_vbh = bgfx::createVertexBuffer(
                bgfx::makeRef(scene->mMeshes[0]->mVertices, scene->mMeshes[0]->mNumVertices * sizeof(aiVector3D)),
                ms_layout);

            std::vector<unsigned short> indices;
            indices.reserve(scene->mMeshes[0]->mNumFaces * 3);
            for (unsigned int i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
            {
                const aiFace& face = scene->mMeshes[0]->mFaces[i];
                assert(face.mNumIndices == 3 && "Face must be triangulated");
                indices.push_back(face.mIndices[0]);
                indices.push_back(face.mIndices[1]);
                indices.push_back(face.mIndices[2]);
            }
            m_ibh = bgfx::createIndexBuffer(
                bgfx::makeRef(indices.data(), indices.size() * sizeof(unsigned short)));
        }
        // Create 11x11 boxes
        for (uint32_t yy = 0; yy < 11; ++yy)
        {
            for (uint32_t xx = 0; xx < 11; ++xx)
            {
                auto box = std::make_shared<Box>();
                box->init();
                
                // Create a translation matrix
                float mtx[16];
                // Set translation matrix
                bx::mtxTranslate(mtx,
                    -15.0f + static_cast<float>(xx) * 3.0f,
                    -15.0f + static_cast<float>(yy) * 3.0f,
                    0.0f);
                box->setTransform(mtx);
                
                m_boxes.push_back(box);
            }
        }

        m_timeOffset = bx::getHPCounter();
        return 0;
    }

    void render(const int width, const int height)
    {
        const auto time = static_cast<float>((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

        constexpr bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        constexpr bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

        // Set view and projection matrix for view 0.
        {
            float view[16];
            bx::mtxLookAt(view, eye, at);

            float proj[16];
            bx::mtxProj(proj, 60.0f, static_cast<float>(width) / static_cast<float>(height),
                0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(kClearView, view, proj);

            // Set view 0 default viewport.
            bgfx::setViewRect(kClearView, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));
        }

        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view
        // 0.
        bgfx::touch(kClearView);

        // Update and render all boxes
        int index = 0;
        for (uint32_t yy = 0; yy < 11; ++yy)
        {
            for (uint32_t xx = 0; xx < 11; ++xx)
            {
                auto& box = m_boxes[index++];

                // Create rotation and translation matrices
                float mtxRot[16];
                float mtxTrans[16];
                float mtx[16];

                // Set rotation matrix based on time
                bx::mtxRotateXY(mtxRot,
                    time + static_cast<float>(xx) * 0.21f,
                    time + static_cast<float>(yy) * 0.37f);

                // Set translation matrix
                bx::mtxTranslate(mtxTrans,
                    -15.0f + static_cast<float>(xx) * 3.0f,
                    -15.0f + static_cast<float>(yy) * 3.0f,
                    0.0f);

                // Combine rotation and translation
                bx::mtxMul(mtx, mtxRot, mtxTrans);

                // Update box matrix
                box->setTransform(mtx);

                // Render the box
                box->render(&material);
            }
        }

        constexpr uint64_t state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | UINT64_C(0) // 0 (default) for TriangleList
            ;

        // Render suzanne
        // Calculate and set transform
        float mtxRot[16];
        float mtxTrans[16];
        float mtxScale[16];
        float mtx[16];

        // Create transformation matrices
        bx::mtxScale(mtxScale, 2.0f);                                // Scale by 2.0
        bx::mtxRotateXY(mtxRot, time + 0.21f, time + 0.37f);         // Rotation over time
        bx::mtxTranslate(mtxTrans, -12.0f, -12.0f, -5.0f);           // Translation position

        // Combine matrices: first scale, then rotate, then translate
        float tempMtx[16];
        bx::mtxMul(tempMtx, mtxScale, mtxRot);  // Apply scaling then rotation
        bx::mtxMul(mtx, tempMtx, mtxTrans);     // Apply translation last

        bgfx::setTransform(mtx);

        // Set vertex and index buffer
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        bgfx::setState(state);

        // Submit primitive for rendering
        bgfx::submit(0, m_simple_program);

        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        // Commented line for logo from logo.h in
        // https://github.com/jpcy/bgfx-minimal-example/blob/master/helloworld.cpp
        // bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2
        // / 16), 6) - 6, 40, 12, s_logo, 160);
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx::dbgTextPrintf(0, 1, 0x0f,
                            "Color can be changed with ANSI "
                            "\x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f,
                            "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    "
                            "\x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f,
                            "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    "
                            "\x1b[;15m    \x1b[0m");
        const bgfx::Stats *stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
                            stats->width, stats->height, stats->textWidth, stats->textHeight);
        // Enable stats or debug text.
        // WARNING: Enabling debug text will disable the rendering (since resources overlap GPU-side)
        // bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx::frame();
    }

    void reset()
    {
        // Clear all boxes
        m_boxes.clear();
        
        // Reset shared resources
        Box::resetShared();

        // TODO: Add proper destructor for material
        bgfx::destroy(material.shader);
        bgfx::destroy(m_simple_program);
        bgfx::destroy(m_vbh);
        bgfx::destroy(m_ibh);
        bgfx::shutdown();
    }

    static bgfx::ShaderHandle loadShader(const char *FILENAME)
    {
        const char* shaderPath = "???";

        switch(bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
            case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
            case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
            case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
            case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
            case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
            default:                             shaderPath = ""; break;
        }

        size_t shaderLen = strlen(shaderPath);
        size_t fileLen = strlen(FILENAME);
        char *filePath = static_cast<char *>(malloc(shaderLen + fileLen));
        strcpy(filePath, shaderPath);
        strcat(filePath, FILENAME);

        FILE *file = fopen(filePath, "rb");
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
        fread(mem->data, 1, fileSize, file);
        mem->data[mem->size - 1] = '\0';
        fclose(file);
        free(filePath);

        return bgfx::createShader(mem);
    }
} // namespace Abyss::renderer
