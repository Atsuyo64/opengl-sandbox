#include "sphere_scene.hpp"
#include "shapes/spheres.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "stb_image.h"

void Sphere_Scene::attach()
{
    // glGetIntegerv(GL_TEXTURE_FREE)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);

    shaders = new Shader[3]{{"assets/shaders/mesh.vert", "assets/shaders/mesh.frag"},
                            {"assets/shaders/mesh.vert", "assets/shaders/meshToFlatNorm.geom", "assets/shaders/mesh.frag"},
                            {"assets/shaders/mesh.vert", "assets/shaders/meshToLines.geom", "assets/shaders/mesh.frag"}};
    shader_MVP_locations[0] = glGetUniformLocation(shaders[0].get_program_ID(), "MVP");
    shader_MVP_locations[1] = glGetUniformLocation(shaders[1].get_program_ID(), "MVP");
    shader_MVP_locations[2] = glGetUniformLocation(shaders[2].get_program_ID(), "MVP");

    texture = new Texture("assets/earth.jpg");

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    should_generate_sphere = true; // or else cause GL_OUT_OF_MEMORY...
}

void Sphere_Scene::detach()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete[] shaders;
    delete texture;
    // shaders = nullptr;
}

void Sphere_Scene::update(glm::mat4 const &view, glm::mat4 const &projection, float dt)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    if (should_generate_sphere)
    {
        should_generate_sphere = false;
        Shape::UVSphere sphere{(size_t)stack, (size_t)sector};
        num_indices = sphere.indices.size();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sphere.mesh.size() * sizeof(Mesh::VertexData), sphere.mesh.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void *)(0 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void *)(6 * sizeof(float)));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned), &sphere.indices[0], GL_DYNAMIC_DRAW);
    }

    glEnable(GL_DEPTH_TEST);

    glm::mat4 MVP{projection * view};
    if (show_faces)
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        if (!no_smooth_shading)
            shaders[0].use();
        else
            shaders[1].use();

        texture->bind_to_sampler(0);

        glUniformMatrix4fv(shader_MVP_locations[0], 1, GL_FALSE, glm::value_ptr(MVP));
        shaders[0].setUniformInt("MYTEXTURE",0);
        
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        //texture->unbind_to_sampler();
    }
    if (show_lines && false)
    {
        shaders[2].use();
        glUniformMatrix4fv(shader_MVP_locations[1], 1, GL_FALSE, glm::value_ptr(MVP));

        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    }

    shaders[0].unuse();
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Sphere_Scene::ImGUIRender()
{
    should_generate_sphere |= ImGui::SliderInt("Stacks", &stack, 2, 100);
    should_generate_sphere |= ImGui::SliderInt("Sectors", &sector, 3, 100);
    ImGui::Checkbox("Show Faces", &show_faces);
    ImGui::Checkbox("No smooth shading", &no_smooth_shading);
    ImGui::Checkbox("Show Lines", &show_lines);
}
