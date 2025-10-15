#include "Shader.h"

std::string readFile(const char* filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
        return ""; // Return an empty string to indicate failure
    }
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	std::cout << "Shader constructor reached" << std::endl;

	std::string vertexCode = readFile(vertexPath);
	std::string fragmentCode = readFile(fragmentPath);

	std::cout << vertexCode << std::endl;
	std::cout << fragmentCode << std::endl;

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	std::cout << vShaderCode << std::endl;
	std::cout << fShaderCode << std::endl;

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::use() const{
	glUseProgram(ID);
}

void Shader::uploadInt(const char* name, int value) const {
	int location = glGetUniformLocation(ID, name);
	glUniform1i(location, value);
}
void Shader::uploadFloat(const char* name, float value) const {
	int location = glGetUniformLocation(ID, name);
	glUniform1f(location, value);
}
void Shader::uploadVec3(const char* name, float x, float y, float z) const {
	int location = glGetUniformLocation(ID, name);
	glUniform3f(location, x, y, z);
}
void Shader::uploadVec3(const char* name, glm::vec3 value) const {
	int location = glGetUniformLocation(ID, name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}
void Shader::uploadMat4(const char* name, glm::mat4 value) const {
	int location = glGetUniformLocation(ID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}