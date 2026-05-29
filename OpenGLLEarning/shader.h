#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    Shader() {}
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMatrix4f(const std::string& name, const glm::mat4 &value);
    void setVector4f(const std::string& name, const glm::vec4 &value);
    void setMatrix3f(const std::string& name, const glm::mat3 &value);
    void setVector3f(const std::string& name, const glm::vec3 &value);
    void setMatrix2f(const std::string& name, const glm::mat2 &value);
    void setVector2f(const std::string& name, const glm::vec2 &value);
};