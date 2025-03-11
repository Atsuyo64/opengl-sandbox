#pragma once
#include "virtualScene.hpp"
#include "glad.h"
#include "openglUtils/shader.hpp"

class Pyramid_Scene : public VirtualScene
{
public:
    void attach() override;
    void detach() override;
    void update(glm::mat4 const &view, glm::mat4 const &projection, float dt) override;
    void ImGUIRender() override;

private:
    struct MyVertexData
    {
        float pos[2];
        float color[4];
    };
    GLuint VAO{};
    GLuint VBO{};
    GLuint EBO_pyramid{};
    GLuint EBO_mesh{};
    GLint shader_MVP_location{};
    Shader* shader{nullptr};
    static const unsigned EBO_pyramid_data[12];
    static const unsigned EBO_mesh_data[12];
    static const MyVertexData vertex_data[4];
};