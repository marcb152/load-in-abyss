//
// Created by marc on 14/04/25.
//

#include "renderer.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include "bgfx/platform.h"
#include "box.hpp"
#include "bx/readerwriter.h"
#include "bx/timer.h"
#include "camera.hpp"
#include "easy_matrix.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imported_mesh.hpp"
#include "input.hpp"

namespace Abyss::renderer
{
    int64_t m_timeOffset;

    // Scene management
    std::vector<std::shared_ptr<Box>> m_boxes;

    static bgfx::ShaderHandle loadShader(const char *FILENAME);
    const bgfx::ViewId kClearView = 0;

    // Suzanne model
    std::shared_ptr<ImportedMesh> m_suzanne;

    // Camera
    std::unique_ptr<Camera> m_camera = std::make_unique<Camera>();

    Material material = {};
    Material simpleMaterial = {};

    bool cursor_enabled = true;
    double prev_time = 0.0;

    void enable_cursor_callback([[maybe_unused]]GLFWwindow* window)
    {
        cursor_enabled = !cursor_enabled;
        Input::setCursorVisible(cursor_enabled);
    }

    int init(bgfx::Init init)
    {
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();

        if (!bgfx::init(init))
            return 1;
        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);

        // Initialize shared box resources
        Box::initShared();
        
        // Create program from shaders.
        // load all programs in shaders directory
        bgfx::ShaderHandle vsh = loadShader("vs_cubes.sc.bin");
        bgfx::setName(vsh, "vs_cubes");
        bgfx::ShaderHandle fsh = loadShader("fs_cubes.sc.bin");
        bgfx::setName(fsh, "fs_cubes");
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

        vsh = loadShader("vs_phong_light.sc.bin");
        bgfx::setName(vsh, "vs_phong_light");
        fsh = loadShader("fs_phong_light.sc.bin");
        bgfx::setName(fsh, "fs_phong_light");
        simpleMaterial.shader = bgfx::createProgram(vsh, fsh, true);
        simpleMaterial.state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CCW
            | BGFX_STATE_MSAA;

        // Load Suzanne model using ImportedMesh
        m_suzanne = std::make_shared<ImportedMesh>("models/suzanne.obj");
        if (!m_suzanne)
        {
            std::cerr << "Failed to create suzanne mesh" << std::endl;
            return -1;
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

        Input::bind(GLFW_KEY_LEFT_CONTROL, enable_cursor_callback);
        m_timeOffset = bx::getHPCounter();
        return 0;
    }

    void render(const int width, const int height)
    {
        const auto time = (bx::getHPCounter() - m_timeOffset) / static_cast<double>(bx::getHPFrequency());
        const auto deltaTime = time - prev_time;

        constexpr bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        constexpr bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

        // Set view and projection matrix for view 0.
        {
            // TODO: We go faster in diagonal!!
            if( Input::keys[GLFW_KEY_W] )
            {
                m_camera->Translate( { 0.0f,0.0f,deltaTime } );
            }
            if( Input::keys[GLFW_KEY_A] )
            {
                m_camera->Translate( { -deltaTime,0.0f,0.0f } );
            }
            if( Input::keys[GLFW_KEY_S] )
            {
                m_camera->Translate( { 0.0f,0.0f,-deltaTime } );
            }
            if( Input::keys[GLFW_KEY_D] )
            {
                m_camera->Translate( { deltaTime,0.0f,0.0f } );
            }
            if( Input::keys[GLFW_KEY_SPACE] )
            {
                m_camera->Translate( { 0.0f,deltaTime,0.0f } );
            }
            if( Input::keys[GLFW_KEY_LEFT_SHIFT] )
            {
                m_camera->Translate( { 0.0f,-deltaTime,0.0f } );
            }
            Input::updateCursor();
            if(!Input::getCursorVisible())
            {
                m_camera->Rotate(static_cast<float>(Input::mouseXDelta), static_cast<float>(Input::mouseYDelta));
            }

            float proj[16];
            bx::mtxProj(proj, 60.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f,
                        bgfx::getCaps()->homogeneousDepth, bx::Handedness::Left);
            bgfx::setViewTransform(kClearView, glm::value_ptr(m_camera->GetMatrix()), proj);

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
                    .rotateXY(
                        time + static_cast<float>(xx) * 0.21f,
                        time + static_cast<float>(yy) * 0.37f)
                    .translate(
                        -15.0f + static_cast<float>(xx) * 3.0f,
                        -15.0f + static_cast<float>(yy) * 3.0f,
                        0.0f);

                // Update box matrix
                box->setTransform(easyMatrix.getMatrix());

                // Render the box
                box->render(&material, kClearView);
            }
        }

        // Render suzanne using ImportedMesh
        EasyMatrix easyMatrix = {};
        easyMatrix
            .scale(5.0f)
            .rotateXY(time + 0.21f, time + 0.37f)
            .translate(-8.0f, -8.0f, -5.0f);

        // Set the transform on the mesh
        m_suzanne->setTransform(easyMatrix.getMatrix());

        // Render the mesh with the material
        m_suzanne->render(&simpleMaterial, kClearView);

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx::frame();
        prev_time = time;
    }

    void resize(const int width, const int height)
    {
        bgfx::renderFrame();
        bgfx::reset(width, height);
        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
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
        // +1 for '\0'
        char* filePath = static_cast<char*>(malloc(shaderLen + fileLen + 1));
        strcpy(filePath, shaderPath);
        strcat(filePath, FILENAME);

        FILE *file = fopen(filePath, "rb");
        if (file == nullptr)
        {
            std::cerr << "Could not open shader file: " << filePath << std::endl;
            free(filePath);
            return BGFX_INVALID_HANDLE;
        }
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
