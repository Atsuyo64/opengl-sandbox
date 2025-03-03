#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <cstddef>

// TODO: re-work !

class Mesh
{
public:
    struct VertexData
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textCoord;
        VertexData() {}
        // VertexData(glm::vec3 &&position_, glm::vec3 &&normal_, glm::vec2 &&textCoord_) : position{std::move(position_)},
        //                                                                              normal{std::move(normal_)},
        //                                                                              textCoord{std::move(textCoord_)} {}
        // VertexData(glm::vec3 &position_, glm::vec3 &normal_, glm::vec2 &textCoord_) : position{position_},
        //                                                                               normal{normal_},
        //                                                                               textCoord{textCoord_} {}
        VertexData(glm::vec3 position_, glm::vec3 normal_, glm::vec2 textCoord_) : position{position_},
                                                                                      normal{normal_},
                                                                                      textCoord{textCoord_} {}
    };

    // Mesh(glm::vec3 *positions, glm::vec3 *normals, glm::vec2 *textCoords, size_t count) : data_ptr{new std::vector<VertexData>{count}}
    //{
    //     // todo: check for nullptr !
    //     for (int i{}; i < count; ++i)
    //         data_ptr->emplace_back(*positions++, *normals++, *textCoords++);
    // }
    //
    // Mesh(std::vector<VertexData> &&mesh) : data_ptr{new std::vector(std::move(mesh))} {}
    //
    // std::shared_ptr<std::vector<VertexData>> get_mesh() const { return data_ptr; }

private:
    std::shared_ptr<std::vector<VertexData>> data_ptr;
};