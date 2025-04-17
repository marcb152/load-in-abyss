/// ==========================================================================
/// Contains code based on the Igneous Engine.
/// Copyright (c) MissingBitStudios
/// Original source available at https://github.com/MissingBitStudios/igneous,
/// licensed under CC-BY-4.0 (https://creativecommons.org/licenses/by/4.0/).
///
/// Modifications made by Marc for Load In Abyss.
/// This modified file, as part of Load In Abyss, is licensed under AGPLv3.
/// ==========================================================================

#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include <bgfx/bgfx.h>

#include "material.hpp"

namespace Abyss
{
    struct Mesh
    {
        bgfx::VertexBufferHandle vbh;
        bgfx::IndexBufferHandle ibh;
        MaterialHandle material;
    };

    struct Model
    {
        std::vector<Mesh> meshes;
    };

    typedef Model* ModelHandle;
} // end namespace Abyss

#endif //MODEL_HPP
