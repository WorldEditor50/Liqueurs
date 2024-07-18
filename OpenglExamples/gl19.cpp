#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>
#include "gl_def.h"
#include "bmp.hpp"

KERNEL_CODE(vertexCode, #version 330 core \n
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            out vec2 TexCoord;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            void main()
            {
                gl_Position = projection*view*model*vec4(aPos, 1.0f);
                TexCoord = vec2(aTexCoord.x, aTexCoord.y);
            });

KERNEL_CODE(fragmentCode, #version 330 core \n
            out vec4 FragColor;
            in vec3 ourColor;
            in vec2 TexCoord;
            // 定义一个全局的2D纹理采样器
            uniform sampler2D texture1;
            uniform sampler2D texture2;
            void main()
            {
                // GLSL内建的mix函数需要接受两个值作为参数，并对它们根据第三个参数进行线性插值。
                FragColor = mix(texture(texture1, TexCoord),
                                texture(texture2,
                                        vec2(-TexCoord.x,TexCoord.y)),
                                0.2);
            });


const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

// 回调函数，每当窗口改变大小，视口大小也跟随改变
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0, 0, width, height);
    return;
}

// 输入 在GLFW中实现一些输入控制
void processInput(GLFWwindow *window)
{
    // 是否按下了返回键(Esc)
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // 把WindowShouldClose属性设置为 true来关闭GLFW
        glfwSetWindowShouldClose(window,true);
    }
    return;
}
// ************** opengl 具体实现 ************
// 顶点数据
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// glm::vec3数组中定义10个立方体位置：
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Mac OS X系统

    // 创建一个GLFW窗口对象
    GLFWwindow* window = glfwCreateWindow(screenWidth ,screenHeight,"texture",NULL,NULL);
    if (window == NULL) {
        std::cout<< "Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    // 窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    // 初始化GLAD(初始化OpenGL函数指针)
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return -1;
    }
    // 着色器类
    Shader shader;
    shader.compile(vertexCode, fragmentCode);
    // 顶点数组对象 VAO
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);   // 创建顶点数组对象
    glBindVertexArray(VAO);      // 绑定顶点数组对象
    // 顶点缓冲对象 VBO
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);                                       // 绑定顶点缓冲对象
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);  // 把之前定义的顶点数据复制到缓冲的内存中

    // 链接顶点属性 （告诉 gpu 如何解析内存中的顶点数据）
    // 0 位置坐标
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    // 1 纹理坐标
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1,texture2;
    // 纹理对象1 texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    // 设置纹理图像环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片
    BMP::Image textureImage1 = BMP::load("./crystalmaiden.bmp", 3);
    unsigned char *data = textureImage1.data;
    int width = textureImage1.w;
    int height = textureImage1.h;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    shader.enable();
    // 纹理对象2 texture2
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    // 设置纹理图像环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    // 重复
    // 设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片
    BMP::Image textureImage2(height, width, 4);
    textureImage2.fill(120, 0, 0, 120);
    data = textureImage2.data;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // 设置采样器对应的纹理单元（unform）
    shader.enable();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 透视矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    // 视口
    glViewport(0,0,screenWidth ,screenHeight);

    // 视口跟随窗口大小改变
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 输入 （用户）
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.enable();
        // 创建转换
        glm::mat4 view = glm::mat4(1.0f);
        float radius = 35.0f;  // 旋转半径
        float camX = std::sin(glfwGetTime())*radius;
        float camZ = std::cos(glfwGetTime())*radius;
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("view", view);

        //
        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++) {
            // 计算每个对象的模型矩阵，并在绘制前将其传递给shader
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f*(float)glfwGetTime()*i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // 检查并调用事件，交换缓冲 （默认操作）
        /* 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
         * 它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。*/
        glfwSwapBuffers(window);
        // 有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态
        glfwPollEvents();
    }
    // 解绑 VAO  VBO 着色器程序对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}
