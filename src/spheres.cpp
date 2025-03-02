#include "spheres.hpp"

using namespace Shape;

#define pi (3.141592653589793f)

Shape::UVSphere::UVSphere(size_t stacks, size_t sectors) : mesh{}, indices{}
{
    assert(stacks > 1 && sectors > 2);
    mesh.reserve(get_num_vertices(stacks, sectors));
    indices.reserve(3 * get_num_triangles(stacks, sectors));
    mesh.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f));
    for (size_t stack{}; stack < stacks - 1; ++stack)
    {
        float phi = pi * 0.5 - pi * (stack+1) / stacks;
        float cos_phi = cos(phi);
        float sin_phi = sin(phi);
        for (size_t sector{}; sector < sectors; ++sector)
        {
            float theta = 2.0f * pi * sector / sectors;
            float x{cos_phi * (float)cos(theta)};
            float y{sin_phi};
            float z{cos_phi * (float)sin(theta)};
            glm::vec3 pos{x, y, z};
            //float u = 0.5f + atan2(pos.z,pos.x)/(2.0f*pi);
            //float v = 0.5f + asin(pos.y)/pi;
            float u = (float)sector / (float)sectors;
            float v = 0.5f + phi/pi;
            mesh.emplace_back(pos, pos, glm::vec2(u,v));
        }
    }
    mesh.emplace_back(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.0f));
    for (size_t i{}; i < sectors; ++i)
    {
        indices.emplace_back(0);
        indices.emplace_back(i+1);
        indices.emplace_back(((i + 1) % sectors)+1);
    }

    for (size_t stack{1}; stack < stacks - 1; ++stack)
    {
        size_t offset = (stack-1) * sectors + 1;
        for (size_t sector{0}; sector < sectors; ++sector)
        {
            bool last_columns = sector == sectors - 1;
            size_t top_left = offset + sector;
            size_t top_right= last_columns ? offset : top_left + 1;
            size_t bot_left = top_left + sectors;
            size_t bot_right= last_columns ? offset + sectors : bot_left + 1;
            indices.emplace_back(top_left);
            indices.emplace_back(bot_left);
            indices.emplace_back(bot_right);

            indices.emplace_back(top_left);
            indices.emplace_back(bot_right);
            indices.emplace_back(top_right);
        }
    }

    size_t offset = (stacks-2) * sectors + 1;
    for (size_t i{}; i < sectors; ++i)
    {
        indices.emplace_back(offset + i);
        indices.emplace_back(offset + sectors);
        indices.emplace_back(i == sectors - 1 ? offset : offset + i + 1);
    }
}

size_t Shape::UVSphere::get_num_vertices(size_t stacks, size_t sectors)
{
    return 2 + (stacks - 1) * sectors;
}

size_t Shape::UVSphere::get_num_triangles(size_t stacks, size_t sectors)
{
    return 2 * stacks * sectors - 2 * sectors;
}