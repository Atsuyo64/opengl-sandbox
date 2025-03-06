class VirtualScene
{
public:
    virtual void attach() = 0;
    virtual void detach() = 0;
    virtual void update(float dt) = 0;
    virtual void ImGUIRender() = 0;
};

struct VirtualSceneContainer {
    VirtualScene* scene;
    bool enabled{false};
    char const* name;
};