#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "Shader.h"


class Camera
{
private:
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 camMatrix;
    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::vec3 initPos;
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    int width;
    int height;

    float speed = 0.1f;
    float sensitivity = 1.0f;
    bool firstClick = true;

    Camera(int width, int height, glm::vec3 position);

    void reset();
    void Matrix(float FOVdeg, float neaerPlane, float farPlane);
    void Upload(Shader& shader, const char* camMatrix) const;
    void Inputs(GLFWwindow* window);
};
