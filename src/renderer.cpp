//
// Created by marc on 14/04/25.
//

#include "renderer.hpp"

#include <cassert>

#include "box.hpp"
#include "imported_mesh.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

#include <memory>
#include <vector>

#include "bgfx/platform.h"
#include "bx/readerwriter.h"
#include "bx/timer.h"
#include "easy_matrix.hpp"

namespace Abyss::renderer
{
    int64_t m_timeOffset;

    // Scene management
    std::vector<std::shared_ptr<Box>> m_boxes;

    static bgfx::ShaderHandle loadShader(const char *FILENAME);
    const bgfx::ViewId kClearView = 0;

    // Suzanne model
    std::shared_ptr<ImportedMesh> m_suzanne;

    Material material = {};
    Material simpleMaterial = {};

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
        simpleMaterial.shader = bgfx::createProgram(vsh, fsh, true);
        simpleMaterial.state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA;

        // Load Suzanne model using ImportedMesh
        m_suzanne = std::make_shared<ImportedMesh>("models/suzanne.obj");
        if (!m_suzanne)
        {
            std::cerr << "Failed to create suzanne mesh" << std::endl;
            return false;
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
                EasyMatrix easyMatrix = {};
                easyMatrix
                    .RotateXY(
                        time + static_cast<float>(xx) * 0.21f,
                        time + static_cast<float>(yy) * 0.37f)
                    .Translate(
                        -15.0f + static_cast<float>(xx) * 3.0f,
                        -15.0f + static_cast<float>(yy) * 3.0f,
                        0.0f);

                // Update box matrix
                box->setTransform(easyMatrix.GetMatrix());

                // Render the box
                box->render(&material);
            }
        }

        // Render suzanne using ImportedMesh
        EasyMatrix easyMatrix = {};
        easyMatrix
            .Scale(2.0f)
            .RotateXY(time + 0.21f, time + 0.37f)
            .Translate(-12.0f, -12.0f, -5.0f);

        // Set the transform on the mesh
        m_suzanne->setTransform(easyMatrix.GetMatrix());

        // Render the mesh with the material
        m_suzanne->render(&simpleMaterial);

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

        // Release suzanne mesh
        m_suzanne.reset();
    
        // TODO: Add proper destructor for material
        bgfx::destroy(material.shader);
        bgfx::destroy(simpleMaterial.shader);
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
