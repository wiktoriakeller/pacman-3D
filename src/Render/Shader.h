#pragma once
#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <unordered_map>
#include <map>
#include <memory>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
	~Shader();
	void Use();
	void Unuse();

	GLint GetUniformLocation(const std::string& name);
	void SetUniform(const std::string& name, GLint value);
	void SetUniform(const std::string& name, GLfloat value);
	void SetUniform(const std::string& name, glm::mat4 value);
	void SetUniform(const std::string& name, glm::fvec4 value);
	void SetUniform(const std::string& name, glm::fvec3 value);
	void SetUniform(const std::string& name, glm::fvec2 value);
	void SetUniform(const std::string& name, glm::mat3 value);

private:
	GLuint programID;
	std::unordered_map<std::string, GLint> uniformLocations;

	std::string LoadShaderSource(const std::string& filePath);
	GLuint LoadShader(GLenum shaderType, const std::string& filePath);
	void LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);
};

inline std::map<std::string, std::shared_ptr<Shader>> shaderMap;