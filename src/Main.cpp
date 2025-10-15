#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "Geometry.h"
#include "Physics.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <functional>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

bool init() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#ifndef __EMSCRIPTEN__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ES_API);
#endif
    return true;
}


int wwidth = 800, wheight = 600;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* window = NULL;
float deltaTime;
float lastFrameTime;
Camera* cPtr;
Shader* sPtr;
World* wPtr;
std::vector<Planet*>* pPtr;
glm::vec3 blue;
glm::vec3 red;
glm::vec3 green;
glm::vec3 orange;
bool running = true;

void mainLoop() {
    if (running) {
        glfwPollEvents();
        float currentFrameTime = glfwGetTime(); 
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime; 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cPtr->Inputs(window);
        cPtr->Matrix(60.0f, 0.01f, 100.0f);
        sPtr->use();
        cPtr->Upload(*(sPtr), "cMatrix");
        wPtr->update(deltaTime);
        sPtr->uploadVec3("myColor", blue);
        glm::mat4 model1 = (*pPtr)[0]->getModel();
        sPtr->uploadMat4("wMatrix", model1);
        (*pPtr)[0]->draw();
        sPtr->uploadVec3("myColor", red);
        glm::mat4 model2 = (*pPtr)[1]->getModel();
        sPtr->uploadMat4("wMatrix", model2);
        (*pPtr)[1]->draw();
        sPtr->uploadVec3("myColor", green);
        glm::mat4 model3 = (*pPtr)[2]->getModel();
        sPtr->uploadMat4("wMatrix", model3);
        (*pPtr)[2]->draw();
        glfwSwapBuffers(window);
    }
}

void resetTime() {
    lastFrameTime = 0.0f;
    deltaTime = 0.0f;
}

void destroy() {
    running = false;
    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;
    delete wPtr;
    wPtr = nullptr;
    if (pPtr != nullptr) {
        for (Planet* p : *pPtr) {
            delete p;
        }
        delete pPtr;
        pPtr = nullptr;
    }
    delete sPtr;
    delete cPtr;
    sPtr = nullptr;
    cPtr = nullptr;
}

void allocPtr(){
    Planet* planet1 = new Planet(10.0f, 1.0f, glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 0.0f));
    Planet* planet2 = new Planet(20.0f, 1.0f, glm::vec3(10.0f, -2.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    Planet* planet3 = new Planet(50.0f, 1.0f, glm::vec3(15.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 2.0f));
    pPtr = new std::vector<Planet*>{ planet1, planet2, planet3};
    sPtr = new Shader("Vert.shader", "Frag.shader");
    cPtr = new Camera(wwidth, wheight, glm::vec3(0.0f, 0.0f, -10.0f));
    wPtr = new World(*pPtr);
    cPtr->Position = glm::vec3(0.0f, 0.0f, 20.0f);
    cPtr->Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
}

void rebuild() {
    init();
    window = glfwCreateWindow(wwidth, wheight, "OpenGL lernen", NULL, NULL);
    glfwMakeContextCurrent(window);
    allocPtr();
}

#ifdef __EMSCRIPTEN__
EXTERN EMSCRIPTEN_KEEPALIVE void wait() {
    running = false;
}
EXTERN EMSCRIPTEN_KEEPALIVE void runAgain() {
    lastFrameTime = glfwGetTime();
    running = true;
}
EXTERN EMSCRIPTEN_KEEPALIVE void nfEnd() {
    running = false;
    destroy();
}
EXTERN EMSCRIPTEN_KEEPALIVE void restart() {
    rebuild();
    lastFrameTime = glfwGetTime();
    running = true;
}
#endif

#ifndef __EMSCRIPTEN__
int main(int argc, char** argv) {
    init();
    window = glfwCreateWindow(wwidth, wheight, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    allocPtr();
    lastFrameTime = 0.0f;
    deltaTime = 0.0f;
    blue = glm::vec3(0.0f, 0.0f, 1.0f);
    red = glm::vec3(1.0f, 0.0f, 0.0f);
    green = glm::vec3(0.0f, 1.0f, 0.0f);
    orange = glm::vec3(0.0f, 0.5f, 0.5f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        mainLoop();
    }
    glfwTerminate();
    return 0;
}
#else
int main() {
    rebuild();
    lastFrameTime = 0.0f;
    deltaTime = 0.0f;
    blue = glm::vec3(0.0f, 0.0f, 1.0f);
    red = glm::vec3(1.0f, 0.0f, 0.0f);
    green = glm::vec3(0.0f, 1.0f, 0.0f);
    orange = glm::vec3(0.0f, 0.5f, 0.5f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    emscripten_set_main_loop(mainLoop, 0, 1);
    destroy();
    return 0;
}
#endif



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    wwidth = width;
    wheight = height;
}