#include "virtualScene.hpp"
#include "glad.h"
#include "openglUtils/shader.hpp"

class Sphere_Scene : public VirtualScene
{
public:
    void attach() override;
    void detach() override;
    void update(glm::mat4 const &view, glm::mat4 const &projection, float dt) override;
    void ImGUIRender() override;
private:
    GLuint VAO,VBO,EBO;
    GLint shader_MVP_locations[2];
    Shader* shaders{nullptr};
    int stack{2}, sector{3};
    bool updated{true};
    bool show_faces{true};
    bool show_lines{false};
    int num_indices;
};