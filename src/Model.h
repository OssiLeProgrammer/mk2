#pragma once
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp" 

class Model
{
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int indexCount;
	glm::mat4 model = glm::mat4(1.0f);
public:
	Model() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}
	void appendBuffer(float* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount) {
		this->indexCount = indexCount;
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void resetModel() {
		model = glm::mat4(1.0f);
	}

	void translate(glm::vec3 translation) {
		model = glm::translate(model, translation);
	}
	void scale(glm::vec3 scale) {
		model = glm::scale(model, scale);
	}
	void rotate(float angle, glm::vec3 axis) {
		model = glm::rotate(model, angle, axis); // Is angle in radians/degrees? What is the answer AI
	}

	glm::mat4 getModel() {
		return model;
	}

	void draw() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
};