#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "backends/imgui_impl_glfw.h"
#include <iostream>

EditorCamera::EditorCamera(GLFWwindow *window)
    : position{glm::vec3(0.0, 0.5, -1.0)},
      direction{glm::vec3(0.0)},
      viewMatrix{glm::lookAt(position, direction, glm::vec3(0.0f, 1.0f, 0.0f))},
      camera_up{glm::normalize(glm::cross(glm::cross(direction - position, glm::vec3(0.0f, 1.0f, 0.0f)), direction - position))}
// prev_cursor_position_callback{glfwSetCursorPosCallback(window,EditorCamera::cursor_position_callback)},
// prev_mouse_button_callback{glfwSetMouseButtonCallback(window,EditorCamera::mouse_button_callback)},
// prev_scroll_callback{glfwSetScrollCallback(window,EditorCamera::scroll_callback)},
// prev_window_size_callback{glfwSetWindowSizeCallback(window,EditorCamera::window_size_callback)}
{

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    projectionMatrix = glm::perspective(glm::radians(45.0f),
                                        (float)w / (float)h,
                                        0.1f, 100.0f);
    viewProjectionMatrix = projectionMatrix * viewMatrix;
    assert(glfwGetWindowUserPointer(window) == NULL && "GLFW window user pointer already used !");
    glfwSetWindowUserPointer(window, this);
    auto prev = glfwSetCursorPosCallback(window, EditorCamera::cursor_position_callback);
    assert(ImGui_ImplGlfw_CursorPosCallback != prev && "Camera must be created BEFORE ImGui_ImplGlfw_InitForOpenGL");
    assert(NULL == prev && "Camera forwarding events not implemented yet...");
    // assert(NULL == glfwSetMouseButtonCallback(window, EditorCamera::mouse_button_callback) && "Camera forwarding events not implemented yet...");
    assert(NULL == glfwSetScrollCallback(window, EditorCamera::scroll_callback) && "Camera forwarding events not implemented yet...");
    assert(NULL == glfwSetWindowSizeCallback(window, EditorCamera::window_size_callback) && "Camera forwarding events not implemented yet...");
}

void EditorCamera::disable()
{
    enabled = false;
}

void EditorCamera::enable()
{
    enabled = true;
}

void EditorCamera::set_two_axis_rotation(bool two_axis_rotation)
{
    this->two_axis_rotation = two_axis_rotation;
}

void EditorCamera::set_two_axis_translation(bool two_axis_translation)
{
    this->two_axis_translation = two_axis_translation;
}

glm::mat4x4 const &EditorCamera::getVPmatrix() const
{
    return viewProjectionMatrix;
}

glm::mat4x4 const &EditorCamera::getViewMatrix() const
{
    return viewMatrix;
}

glm::mat4x4 const &EditorCamera::getProjectionMatrix() const
{
    return projectionMatrix;
}

glm::vec3 const &EditorCamera::get_position() const
{
    return position;
}

glm::vec3 const &EditorCamera::get_center() const
{
    return direction;
}

void EditorCamera::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    EditorCamera *self = (EditorCamera *)glfwGetWindowUserPointer(window);
    if (!self->enabled)
        return;

    int mouse_button_middle_status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (mouse_button_middle_status == GLFW_RELEASE && self->middle_click_pressed)
        self->middle_click_pressed = false;
    else if (mouse_button_middle_status == GLFW_PRESS && !self->middle_click_pressed)
    {
        self->middle_click_pressed = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        self->start_mouse_pos = glm::ivec2(xpos, ypos);
        self->prev_position = self->position;
        self->prev_camera_up = self->camera_up;
        self->prev_direction = self->direction;
    }

    int shift_key_status = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

    if (mouse_button_middle_status == GLFW_PRESS && shift_key_status == GLFW_RELEASE)
    {
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        float delta_mouse_x = 2.0f * glm::two_pi<float>() * (xpos - self->start_mouse_pos.x) / (float)w;
        float delta_mouse_y = 2.0f * glm::two_pi<float>() * (ypos - self->start_mouse_pos.y) / (float)h;
        if (!self->two_axis_rotation)
        {
            if (abs(delta_mouse_x) >= abs(delta_mouse_y))
                delta_mouse_y = 0.0f;
            else
                delta_mouse_x = 0.0f;
        }
        if (self->prev_camera_up.y < 0)
            delta_mouse_y *= -1.0f;
        glm::vec3 c_pos{self->prev_position - self->direction};
        glm::mat4 trans = glm::rotate(glm::mat4{1.0f}, -delta_mouse_x, glm::vec3(0.0f, 1.0f, 0.0f));
        // glm::mat4 trans = glm::rotate(glm::mat4{1.0f},-delta_mouse_x,self->prev_camera_up);
        trans = glm::rotate(trans, delta_mouse_y, glm::cross(c_pos, glm::vec3(0.0f, 1.0f, 0.0f)));
        self->position = glm::vec3(trans * glm::vec4(c_pos, 0.0f)) + self->direction;
        self->camera_up = glm::vec3(trans * glm::vec4(self->prev_camera_up, 0.0));
        self->direction = self->prev_direction; // to undo translation
        self->updateVPmatrix();
    }
    else if (mouse_button_middle_status == GLFW_PRESS && shift_key_status == GLFW_PRESS)
    {
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        float translation_factor = glm::length(self->prev_position - self->prev_direction);
        float delta_mouse_x = translation_factor * (xpos - self->start_mouse_pos.x) / (float)w;
        float delta_mouse_y = translation_factor * (ypos - self->start_mouse_pos.y) / (float)h;
        if (!self->two_axis_translation)
        {
            if (abs(delta_mouse_x) >= abs(delta_mouse_y))
                delta_mouse_y = 0.0f;
            else
                delta_mouse_x = 0.0f;
        }
        auto right_vector = glm::normalize(glm::cross(self->prev_position - self->prev_direction, self->prev_camera_up));
        auto translation_vector = right_vector * delta_mouse_x + self->prev_camera_up * delta_mouse_y;
        self->direction = self->prev_direction + translation_vector;
        self->position = self->prev_position + translation_vector;
        self->camera_up = self->prev_camera_up; // to undo rotation
        self->updateVPmatrix();
    }
}

/*
void EditorCamera::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    return;
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    EditorCamera *self = (EditorCamera *)glfwGetWindowUserPointer(window);
    if (!self->enabled)
        return;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            self->start_mouse_pos = glm::ivec2(xpos, ypos);
            self->prev_position = self->position;
            self->prev_camera_up = self->camera_up;
            self->middle_click_pressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            self->middle_click_pressed = false;
        }
    }
}
*/

void EditorCamera::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    EditorCamera *self = (EditorCamera *)glfwGetWindowUserPointer(window);
    if (!self->enabled)
        return;
    auto dir = self->position - self->direction;
    self->position = (float)pow(0.9, yoffset) * dir + self->direction;
    self->updateVPmatrix();
}

void EditorCamera::window_size_callback(GLFWwindow *window, int width, int height)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    EditorCamera *self = (EditorCamera *)glfwGetWindowUserPointer(window);
    if (!self->enabled)
        return;
    self->projectionMatrix = glm::perspective(glm::radians(45.0f),
                                              (float)width / (float)height,
                                              0.1f, 100.0f);
    self->viewProjectionMatrix = self->viewMatrix * self->projectionMatrix;
}

void EditorCamera::updateVPmatrix()
{
    // FIXME: prevent from rotating around 1,0,0 !
    if (camera_up.y >= 0.0f)
        viewMatrix = glm::lookAt(position, direction, glm::vec3(0.0, 1.0, 0.0));
    else
        viewMatrix = glm::lookAt(position, direction, glm::vec3(0.0, -1.0, 0.0));
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}
