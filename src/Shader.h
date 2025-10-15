#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp" 

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void use() const;
	void uploadInt(const char* name, int value) const;
	void uploadFloat(const char* name, float value) const;
	void uploadVec3(const char* name, float x, float y, float z) const;
	void uploadVec3(const char* name, glm::vec3 value) const;
	void uploadMat4(const char* name, glm::mat4 value) const;
private:
	unsigned int ID;
};