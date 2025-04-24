//
// Created by marc on 14/04/25.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "bgfx/bgfx.h"
#include "box.hpp"

/// \brief The renderer singleton.
/// \details This singleton is used to handle BGFX rendering, and the rendering loop.
/// All Windowing (GLFW) details don't belong here.
namespace Abyss::renderer
{
    /// @brief Initialize the renderer with BGFX parameters
    /// @param init BGFX initialization parameters
    /// @return 0 on success, non-zero error code on failure
    int init(bgfx::Init init);

    /// @brief Render the scene with the current viewport dimensions
    /// @param width The viewport width in pixels
    /// @param height The viewport height in pixels
    void render(int width, int height);

    /// @brief Reset the renderer state and release resources
    /// @details Call this when shutting down or when the rendering context changes
    void reset();
} // Abyss

#endif //RENDERER_HPP
