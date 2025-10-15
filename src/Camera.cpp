#include "Camera.h"

void Camera::reset() {
    Position = this->initPos;
    float yaw = -90.0f;
    float pitch = 0.0f;
    this->Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(int width, int height, glm::vec3 position)
{
    std::cout << "Camera Constructor reached" << std::endl;
    this->initPos = position;
    this->width = width;
    this->height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane)
{
    // Update the view matrix based on the new orientation
    view = glm::lookAt(Position, Position + Orientation, Up);

    // Update the projection matrix
    proj = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Combine the view and projection matrices
    camMatrix = proj * view;
}
void Camera::Upload(Shader &shader, const char *camName) const
{
    shader.uploadMat4(camName, camMatrix);
}

void Camera::Inputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position = Position + speed * Orientation; // Forward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position = Position - speed * glm::normalize(glm::cross(Orientation, Up)); // Left
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position = Position - speed * Orientation; // Backward
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position = Position + speed * glm::normalize(glm::cross(Orientation, Up)); // Right
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position = Position + speed * Up; // Up
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        firstClick = false;
        Position = Position - speed * Up; // Down
    }
#ifndef __EMSCRIPTEN__
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (firstClick)
        {
            glfwSetCursorPos(window, width / 2, height / 2);
            firstClick = false;
        }

        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        double mouseDeltaX = (float)(mouse_x - (width / 2));   // Horizontal mouse delta
        double mouseDeltaY = -(float)(mouse_y - (height / 2)); // Vertical mouse delta

        // Update yaw and pitch based on mouse movement
        GLfloat rotX = mouseDeltaY * sensitivity; // Rotation in the X axis (vertical)
        GLfloat rotY = mouseDeltaX * sensitivity; // Rotation in the Y axis (horizontal)

        // Update pitch (clamping the vertical rotation between -89 and 89 degrees)
        pitch += rotX;
        pitch = glm::clamp(pitch, -89.0f, 89.0f); // Prevent looking too far up or down

        // Update yaw (horizontal rotation, no clamping needed)
        yaw += rotY;

        // Recalculate orientation based on yaw and pitch
        Orientation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        Orientation.y = sin(glm::radians(pitch));
        Orientation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        Orientation = glm::normalize(Orientation); // Normalize the orientation vector
        glfwSetCursorPos(window, width / 2, height / 2);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
#else
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        Position = Position + speed * Up; // Up
    }
#endif
}