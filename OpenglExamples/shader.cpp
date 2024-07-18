#include "shader.h"

void Shader::load(const char *vertexPath, const char *fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstreamd对象可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    compile(vertexCode, fragmentCode);
}

void Shader::compile(const std::string &vertexCode, const std::string &fragmentCode)
{
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 顶点着色器对象
    unsigned int vertexShader;
    // 创建顶点着色器对象
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 把顶点着色器源码附加到顶点着色器对象上
    glShaderSource(vertexShader,1,&vShaderCode,NULL);
    // 编译顶点着色器对象
    glCompileShader(vertexShader);
    // 检测顶点着色器对象编译是否成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if (!success) {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout<<"Error::shader::vertex::compilation_failed\n"<<infoLog<<std::endl;
    }
    // 片段着色器对象
    unsigned int fragmentShader;
    // 创建片段着色器对象
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 把片段着色器源码附加到片段着色器对象上
    glShaderSource(fragmentShader,1,&fShaderCode,NULL);
    // 编译片段着色器对象
    glCompileShader(fragmentShader);
    // 检查片段着色器对象是否编译成功
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout<<"Error::shader::vertex::compilation_failed\n"<<infoLog<<std::endl;
    }
    // 着色器程序对象
    unsigned int shaderProgram;
    // 创建着色器程序对象
    shaderProgram = glCreateProgram();
    // 附加顶点着色器到着色器程序对象上
    glAttachShader(shaderProgram,vertexShader);
    // 附加片段着色器到着色器程序对象上
    glAttachShader(shaderProgram,fragmentShader);
    // 链接着色器程序对象
    glLinkProgram(shaderProgram);
    // 检查着色器程序对象是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    program = shaderProgram;
    // 把着色器对象链接到程序对象以后，删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return;
}

void Shader::enable()
{
    // 使用着色器程序对象
    glUseProgram(program);
    return;
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    return;
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    return;
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    return;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()),
                       1, GL_FALSE, glm::value_ptr(value));
    return;
}
