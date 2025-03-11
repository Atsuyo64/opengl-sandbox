#pragma once
#include "mesh.hpp"
#include <cstddef>

namespace Shape
{
    class UVSphere
    {
    public:
        UVSphere(size_t stacks, size_t sectors);

    //private:
        // size_t stacks;
        // size_t sectors;
        std::vector<Mesh::VertexData> mesh;
        std::vector<unsigned> indices;
    private:
        static size_t get_num_vertices(size_t stacks, size_t sectors);
        static size_t get_num_triangles(size_t stacks, size_t sectors);
    };
}