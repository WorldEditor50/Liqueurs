#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int program;
public:
    Shader():program(0){}
    void load(const char* vertexPath,const char* fragmentPath);
    void compile(const std::string &vertexCode,
                 const std::string &fragmentCode);
    void enable();
    void setBool(const std::string &name,bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name,float value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
};

#endif // SHADER_H
