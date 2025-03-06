#include <cstddef>
#include "glad.h"
#include "glDebugLog.hpp"
#include "initVertexBuffer.hpp"
#include "shader.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spheres.hpp"
#include <iostream>
#include <vector>
#include "virtualScene.hpp"

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

    std::vector<VirtualSceneContainer> scenes{};

    struct MyVertexData
    {
        float pos[2];
        float color[4];
    };
    MyVertexData vertex_data[] = {
        {{0.0, 0.5}, {1.0, 0.0, 0.0, 1.0}},
        {{-0.5, -0.5}, {0.0, 1.0, 0.0, 1.0}},
        {{0.5, -0.5}, {0.0, 0.0, 1.0, 1.0}},
        {{0.0, 0.0}, {0.0, 0.0, 0.0, 1.0}}};
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    VBO = initFloatVertexBuffer(vertex_data, 4, GL_STATIC_DRAW, 2, 4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    unsigned EBO_pyramid_data[]{0, 1, 2, 0, 1, 3, 0, 2, 3, 1, 2, 3};
    GLuint EBO_pyramid;
    glGenBuffers(1, &EBO_pyramid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned), EBO_pyramid_data, GL_STATIC_DRAW);
    unsigned EBO_mesh_data[]{0, 1, 0, 2, 0, 3, 1, 2, 1, 3, 2, 3};
    GLuint EBO_mesh;
    glGenBuffers(1, &EBO_mesh);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned), EBO_mesh_data, GL_STATIC_DRAW);
    Shader shader{"assets/poscolor.vert", "assets/color.frag"};

    GLuint VAO_Sphere, VBO_Sphere, EBO_Sphere;
    glGenVertexArrays(1, &VAO_Sphere);
    glGenBuffers(1, &EBO_Sphere);
    glBindVertexArray(VAO_Sphere);
    glGenBuffers(1, &VBO_Sphere);
    // Shape::UVSphere sphere{2, 3};
    // VBO_Sphere = initFloatVertexBuffer(&sphere.mesh[0], sphere.mesh.size(), GL_STATIC_DRAW, 3, 3, 2);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Sphere);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indices.size() * sizeof(unsigned), &sphere.indices[0], GL_STATIC_DRAW);
    //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //  glBindVertexArray(0);
    //  std::cout << sphere.mesh.size() << " " << sphere.indices.size() << std::endl;
    //  for (auto &&index : sphere.indices)
    //      std::cout << index << " ";
    //  std::cout << std::endl;
    //  for (auto &&index : sphere.mesh)
    //      std::cout << "(" << index.position.x << "," << index.position.y << "," << index.position.z << ") ";
    //  std::cout << std::endl;
    Shader mesh_shader{"assets/mesh.vert", "assets/mesh.frag"};
    int stacks = 2;
    int sectors = 3;

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

    bool show_pyramid = false;
    bool show_sphere = true;
    bool show_demo_window = false;
    double time = glfwGetTime();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); //ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

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



        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Main menu"); // Create a window called "Hello, world!" and append into it.

            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            if (ImGui::Checkbox("Vsynch", &vsynch_enabled))
                glfwSwapInterval(vsynch_enabled);
            
            ImGui::SeparatorText("Scenes");
            
            for(VirtualSceneContainer& container : scenes) {
                if (ImGui::Checkbox(container.name,&container.enabled)) {
                    if(container.enabled) container.scene->attach();
                    else container.scene->detach();
                }
                if (container.enabled) {
                    ImGui::Begin(container.name);
                    container.scene->ImGUIRender();
                    ImGui::End();
                }
            }
            ImGui::Checkbox("Pyramid", &show_pyramid);
            ImGui::Checkbox("Sphere", &show_sphere);

            ImGui::ColorEdit4("Vertex 1 color:", vertex_data[0].color);
            ImGui::ColorEdit4("Vertex 2 color:", vertex_data[1].color);
            ImGui::ColorEdit4("Vertex 3 color:", vertex_data[2].color);

            ImGui::SliderInt("Stacks", &stacks, 2, 100);
            ImGui::SliderInt("Sectors", &sectors, 3, 100);

            // auto vec2 = camera.getVPmatrix()*glm::vec4(vertex_data[0].color[0],vertex_data[0].color[1],vertex_data[0].color[2],vertex_data[0].color[3]);
            // ImGui::Text("%.3f %.3f %.3f %.3f",vertex_data[0].color[0],vertex_data[0].color[1],vertex_data[0].color[2],vertex_data[0].color[3]);
            // ImGui::Text("%.3f %.3f %.3f %.3f",vec2.x/vec2.w,vec2.y/vec2.w,vec2.z/vec2.w,vec2.w);
            auto vec = camera.get_position();
            ImGui::Text("Cam pos: %.3f %.3f %.3f", vec.x, vec.y, vec.z);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }


        double newTime = glfwGetTime();
        for(auto& container : scenes)
            if(container.enabled)
                container.scene->update(newTime - time);
        time = newTime;

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // My render !
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D,rendered_texture);
        // glBindFramebuffer(GL_FRAMEBUFFER,FramebufferName);

        if (show_sphere)
        {
            Shape::UVSphere sphere{(size_t)stacks, (size_t)sectors};
            glBindBuffer(GL_ARRAY_BUFFER, VBO_Sphere);
            // VBO_Sphere = initFloatVertexBuffer(&sphere.mesh[0], sphere.mesh.size(), GL_STATIC_DRAW, 3, 3, 2);
            glBufferData(GL_ARRAY_BUFFER, sphere.mesh.size() * sizeof(Mesh::VertexData), sphere.mesh.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void *)(0 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::VertexData), (void *)(6 * sizeof(float)));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Sphere);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indices.size() * sizeof(unsigned), &sphere.indices[0], GL_STATIC_DRAW);
            glBindVertexArray(VAO_Sphere);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_Sphere);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Sphere);
            mesh_shader.use();
            mesh_shader.setUniformMatrix4x4("MVP", camera.getVPmatrix());
            glEnable(GL_DEPTH_TEST);
            // glEnable(GL_BLEND);
            // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            //glPointSize(10.0f);
            glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);

            // glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
            // glDrawElements(GL_LINES, sphere.indices.size(), GL_UNSIGNED_INT, 0);
        }

        // glBindFramebuffer(GL_FRAMEBUFFER,0);
        // glBindVertexArray(VAO_quad);
        // glBindBuffer(GL_ARRAY_BUFFER,VBO_quad);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_quad);
        // quad_shader.use();

        if (show_pyramid)
        {
            glBindVertexArray(VAO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof(float), vertex_data);
            shader.use();

            shader.setUniformMatrix4x4("MVP", camera.getVPmatrix());
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_mesh);
            glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, 0);
            // //glDrawArrays(GL_TRIANGLES,0,3);
        }

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
