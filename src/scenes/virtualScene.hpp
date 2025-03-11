#pragma once
#include "glm/mat4x4.hpp"

class VirtualScene
{
public:
    virtual void attach() = 0;
    virtual void detach() = 0;
    virtual void update(glm::mat4 const &view, glm::mat4 const &projection, float dt) = 0;
    virtual void ImGUIRender() = 0;

    VirtualScene() {};
};

struct VirtualSceneContainer
{
    VirtualScene *scene;
    bool enabled{false};
    char const *name;
};