#pragma once
// #define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class EditorCamera
{
public:
    EditorCamera(GLFWwindow *window);
    void disable();
    void enable();
    void set_two_axis_rotation(bool two_axis_rotation);
    void set_two_axis_translation(bool two_axis_translation);
    glm::mat4x4 const &getVPmatrix() const;
    glm::mat4x4 const& getViewMatrix() const;
    glm::mat4x4 const& getProjectionMatrix() const;
    glm::vec3 const &get_position() const;
    glm::vec3 const &get_center() const;

private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 camera_up;
    glm::mat4x4 viewMatrix;
    glm::mat4x4 projectionMatrix;
    glm::mat4x4 viewProjectionMatrix;
    glm::vec3 prev_direction;
    bool enabled = true;
    bool middle_click_pressed = false;
    glm::vec3 prev_position;
    glm::vec3 prev_camera_up;
    glm::ivec2 start_mouse_pos;
    bool two_axis_rotation = true;
    bool two_axis_translation = true;
    // const GLFWcursorposfun prev_cursor_position_callback;
    // const GLFWmousebuttonfun prev_mouse_button_callback;
    // const GLFWscrollfun prev_scroll_callback;
    // const GLFWwindowsizefun prev_window_size_callback;

private:
    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    // static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void window_size_callback(GLFWwindow *window, int width, int height);
    void updateVPmatrix();

public:
    EditorCamera(EditorCamera const &) = delete;
    EditorCamera(EditorCamera const &&) = delete;
    EditorCamera &operator=(EditorCamera const &) = delete;
    EditorCamera &operator=(EditorCamera const &&) = delete;
};