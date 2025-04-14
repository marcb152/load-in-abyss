//
// Created by marc on 14/04/25.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "bgfx/bgfx.h"

/// \brief The renderer singleton.
/// \details This singleton is used to handle BGFX rendering, and the rendering loop.
/// All Windowing (GLFW) details don't belong here.
namespace Abyss::renderer
{
    int init(bgfx::Init init);

    void render(int width, int height);

    void reset();
} // Abyss

#endif //RENDERER_HPP
