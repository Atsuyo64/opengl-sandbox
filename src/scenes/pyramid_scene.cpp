#include "pyramid_scene.hpp"
#include "openglUtils/initVertexBuffer.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const unsigned Pyramid_Scene::EBO_pyramid_data[]{0, 1, 2, 0, 1, 3, 0, 2, 3, 1, 2, 3};
const unsigned Pyramid_Scene::EBO_mesh_data[]{0, 1, 0, 2, 0, 3, 1, 2, 1, 3, 2, 3};
const Pyramid_Scene::MyVertexData Pyramid_Scene::vertex_data[]{
    {{0.0, 0.5}, {1.0, 0.0, 0.0, 1.0}},
    {{-0.5, -0.5}, {0.0, 1.0, 0.0, 1.0}},
    {{0.5, -0.5}, {0.0, 0.0, 1.0, 1.0}},
    {{0.0, 0.0}, {0.0, 0.0, 0.0, 1.0}}};

void Pyramid_Scene::attach()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(MyVertexData), vertex_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(MyVertexData),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(MyVertexData),(void*)(offsetof(MyVertexData,color)));

    shader = new Shader{"assets/shaders/poscolor.vert", "assets/shaders/color.frag"};
    shader_MVP_location = glGetUniformLocation(shader->get_program_ID(), "MVP");

    glGenBuffers(1, &EBO_pyramid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned), EBO_pyramid_data, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO_mesh);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned), EBO_mesh_data, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Pyramid_Scene::detach()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO_mesh);
    glDeleteBuffers(1, &EBO_pyramid);
    if (shader)
        delete shader;
}

void Pyramid_Scene::update(glm::mat4 const &view, glm::mat4 const &projection, float dt)
{
    glBindVertexArray(VAO); //Store current VBOs + data + layouts
    
    shader->use();
    glm::mat4 MVP_matrix = projection * view;
    glUniformMatrix4fv(shader_MVP_location, 1, GL_FALSE, glm::value_ptr(MVP_matrix));
    
    glEnable(GL_DEPTH_TEST);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
    glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, 0);
    
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    shader->unuse();
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Pyramid_Scene::ImGUIRender()
{
}
