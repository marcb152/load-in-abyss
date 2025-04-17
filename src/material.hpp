/// ==========================================================================
/// Contains code based on the Igneous Engine.
/// Copyright (c) MissingBitStudios
/// Original source available at https://github.com/MissingBitStudios/igneous,
/// licensed under CC-BY-4.0 (https://creativecommons.org/licenses/by/4.0/).
///
/// Modifications made by Marc for Load In Abyss.
/// This modified file, as part of Load In Abyss, is licensed under AGPLv3.
/// ==========================================================================

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <vector>

#include <bgfx/bgfx.h>

namespace Abyss
{
    enum MATERIAL_ATTRIBUTE : uint8_t
    {
        COLOR,
        DIFFUSE,
        NORMAL,
        SPECULAR
    };

    struct Material
    {
        bgfx::ProgramHandle shader;
        uint64_t state;
        std::vector<std::pair<bgfx::UniformHandle, void*>> uniforms;
        std::vector<std::pair<bgfx::UniformHandle, bgfx::TextureHandle>> textures;
    };

    typedef Material* MaterialHandle;
} // end namespace Abyss

#endif //MATERIAL_HPP
