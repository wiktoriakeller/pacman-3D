#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint geometryShader = 0;

	vertexShader = LoadShader(GL_VERTEX_SHADER, vertexPath);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	if (geometryPath != "")
		geometryShader = LoadShader(GL_GEOMETRY_SHADER, geometryPath);

	LinkProgram(vertexShader, fragmentShader, geometryShader);

	glDetachShader(programID, vertexShader);
	glDetachShader(programID, fragmentShader);
	glDetachShader(programID, geometryShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

Shader::~Shader() {
	glDeleteProgram(programID);
}

void Shader::Use() {
	glUseProgram(programID);
}

void Shader::Unuse() {
	glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string& name) {
	if (uniformLocations.find(name) != uniformLocations.end())
		return uniformLocations[name];

	GLint location = glGetUniformLocation(programID, name.c_str());

	if (location == -1)
		std::cout << "Warning shader uniform location: " << name << " does not exist\n";

	uniformLocations[name] = location;
	return location;
}

void Shader::SetUniform(const std::string& name, GLint value) {
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, GLfloat value) {
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, glm::fvec4 value) {
	glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, glm::fvec3 value) {
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, glm::mat4 value) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

std::string Shader::LoadShaderSource(const std::string& filePath) {
	std::ifstream file;
	std::string source = "";
	file.open(filePath);

	if (file) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		source = buffer.str();
	}
	else {
		std::cout << "Error shader file does not exist " << filePath << "\n";
	}

	return source;
}

GLuint Shader::LoadShader(GLenum shaderType, const std::string& filePath) {
	std::string source = LoadShaderSource(filePath);
	const GLchar* src = (const GLchar*) source.c_str();
	GLuint shader = glCreateShader(shaderType);

	GLint result;
	GLchar infoLog[512];

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Error shader compilation failed\n" << infoLog << "\n";
		glDeleteShader(0);
		return 0;
	}

	return shader;
}

void Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader) {
	GLint result;
	GLchar infoLog[512];

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);

	if (geometryShader)
		glAttachShader(programID, geometryShader);

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &result);

	if (result == GL_FALSE) {
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		std::cout << "Error shader program linking failed\n" << infoLog << "\n";
	}
}