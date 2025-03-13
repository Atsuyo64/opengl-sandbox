#include <cstddef>
#include "glad.h"
#include "openglUtils/glDebugLog.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include <vector>
#include "scenes/virtualScene.hpp"
#include "scenes/pyramid_scene.hpp"
#include "scenes/sphere_scene.hpp"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char **)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only

    GLFWwindow *window = glfwCreateWindow(1280, 720, "OPENGL SANDBOX", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    bool vsynch_enabled = 1;
    glfwSwapInterval(vsynch_enabled); // Enable vsync

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    set_opengl_log_level(4, true);
    glDebugMessageCallback(opengl_debug_callback, NULL);

    EditorCamera camera{window};

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    io.IniFilename = "build/imgui.ini";
    // io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup scenes
    std::vector<VirtualSceneContainer> scenes{};
    scenes.push_back(VirtualSceneContainer{new Pyramid_Scene(), false, "Pyramid"});
    scenes.push_back(VirtualSceneContainer{new Sphere_Scene(), false, "Sphere"});

    /*
    GLuint FramebufferName;
    glGenBuffers(1,&FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    GLuint rendered_texture;
    glGenTextures(1,&rendered_texture);
    glBindTexture(GL_TEXTURE_2D,rendered_texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1280,720,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,rendered_texture,0);
    GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1,&draw_buffer);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "oskour" <<std::endl;

    GLuint VBO_quad,VAO_quad,EBO_quad;
    glGenVertexArrays(1,&VAO_quad);
    glBindVertexArray(VAO_quad);
    float quad[] = {
        -1,-1,0,0, //pos, uv
        -1, 1,0,1,
         1, 1,1,1,
         1,-1,1,0
    };
    unsigned quad_indices[] = {0,1,2,0,2,3};
    glGenBuffers(1,&VBO_quad);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_quad);
    glBufferData(GL_ARRAY_BUFFER,16*sizeof(float),quad,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
    glGenBuffers(1,&EBO_quad);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_quad);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(unsigned),quad_indices,GL_STATIC_DRAW);
    Shader quad_shader{"assets/quad.vert","assets/quad.frag"};
    */

    ///bool show_sphere = false;
    bool show_demo_window = false;
    double time = glfwGetTime();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            ImGui::Begin("Main menu");

            ImGui::Checkbox("Demo Window", &show_demo_window);
            if (ImGui::Checkbox("Vsynch", &vsynch_enabled))
                glfwSwapInterval(vsynch_enabled);

            ImGui::SeparatorText("Scenes");

            ImGui::PushID("Scenes");
            for (VirtualSceneContainer &container : scenes)
            {
                if (ImGui::Checkbox(container.name, &container.enabled))
                {
                    if (container.enabled)
                        container.scene->attach();
                    else
                        container.scene->detach();
                }
                if (container.enabled)
                {
                    ImGui::Begin(container.name);
                    container.scene->ImGUIRender();
                    ImGui::End();
                }
            }
            ImGui::PopID();

            // auto vec = camera.get_position();
            // ImGui::Text("Cam pos: %.3f %.3f %.3f", vec.x, vec.y, vec.z);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 const &view{camera.getViewMatrix()};
        glm::mat4 const &projection{camera.getProjectionMatrix()};
        double newTime = glfwGetTime();
        for (auto &container : scenes)
            if (container.enabled)
                container.scene->update(view, projection, newTime - time);
        time = newTime;

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D,rendered_texture);
        // glBindFramebuffer(GL_FRAMEBUFFER,FramebufferName);

        // glBindFramebuffer(GL_FRAMEBUFFER,0);
        // glBindVertexArray(VAO_quad);
        // glBindBuffer(GL_ARRAY_BUFFER,VBO_quad);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_quad);
        // quad_shader.use();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
