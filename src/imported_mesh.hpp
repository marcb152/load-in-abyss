//
// Created by marc on 19/04/25.
//

#ifndef IMPORTED_MESH_HPP
#define IMPORTED_MESH_HPP

#include <string>

#include "bgfx/bgfx.h"
#include "mesh.hpp"
#include "material.hpp"

namespace Abyss
{
    class ImportedMesh final : public Mesh
    {
    public:
        /// Constructor
        explicit ImportedMesh(const std::string& filePath);
        /// Destructor
        ~ImportedMesh() override;
        /// Copy constructor
        ImportedMesh(const ImportedMesh &other) = delete;
        /// Copy assignment operator
        ImportedMesh &operator=(const ImportedMesh &other) = delete;
        /// Move constructor
        ImportedMesh(ImportedMesh &&other) = default;
        /// Move assignment operator
        ImportedMesh &operator=(ImportedMesh &&other) = default;

        void init() override;
        void render(MaterialHandle material) override;
        void reset() override;

    private:
        bool loadMesh(const std::string& filePath);

        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;
        bool m_initialized;
    };
} // Abyss

#endif //IMPORTED_MESH_HPP
