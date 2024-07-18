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

using namespace std;

KERNEL_CODE(vertexCode, #version 330 core \n
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 transform;
void main()
{
   gl_Position = transform * vec4(aPos,1.0) ;
   TexCoord = vec2(aTexCoord.x, 1-aTexCoord.y);
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
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x,TexCoord.y)), 0.2);
});

// 回调函数，每当窗口改变大小，视口大小也跟随改变
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

// 输入 在GLFW中实现一些输入控制
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)     // 是否按下了返回键(Esc)
    {
        glfwSetWindowShouldClose(window,true);               // 把WindowShouldClose属性设置为 true来关闭GLFW
    }
}
// ************** opengl 具体实现 ************
    // 顶点数据
float vertices[] = {
    //     ---- 位置 ----    - 纹理坐标 -
     0.5f,  0.5f,  0.0f,     1.0f, 1.0f,   // 右上
     0.5f, -0.5f,  0.0f,     1.0f, 0.0f,   // 右下
    -0.5f,  -0.5f, 0.0f,     0.0f, 0.0f,   // 左下
    -0.5f,   0.5f, 0.0f,     0.0f, 1.0f    // 左上
};

    // 纹理索引
unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
//******************************************
int main()
{
    cout << "纹理" << endl;


// GLFW (窗口)
    glfwInit();                                                         // 初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);                       // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);                       // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);       // 使用核心模式
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Mac OS X系统

    // 创建一个GLFW窗口对象
    GLFWwindow* window = glfwCreateWindow(800,600,"texture",NULL,NULL);
    if(window == NULL)
    {
        std::cout<< "Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);                              // 窗口的上下文设置为当前线程的主上下文

// GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))      // 初始化GLAD(初始化OpenGL函数指针)
    {
        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return -1;
    }

//*********** opengl 具体实现 ******************
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
    // 索引缓存对象 EBO
    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);                                       // 绑定索引缓冲对象
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);  // 把之前定义的索引数据复制到索引缓冲的内存中

    // 链接顶点属性 （告诉 gpu 如何解析内存中的顶点数据）
    // 0 位置坐标
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    // 1 纹理坐标
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1,texture2;
    // 纹理对象1 texture1
    glGenTextures(1,&texture1);
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
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
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
    BMP::Image textureImage2 = BMP::load("./crystalmaiden.bmp", 4);
    data = textureImage2.data;

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // 设置采样器对应的纹理单元（unform）
    shader.enable();
    shader.setInt("texture1",0);
    shader.setInt("texture2",1);

    // 变换
    float scal = 0.1;
    bool flag = true;
    unsigned int transformLoc = glGetUniformLocation(shader.program,"transform");
//**************************




// 视口
    glViewport(0,0,800,600);

// 视口跟随窗口大小改变
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

// 渲染循环
    while(!glfwWindowShouldClose(window))                            // 检查GLFW是否被要求退出
    {
        // 输入 （用户）
        processInput(window);                                        // 输入控制（自定义）

        //************** 渲染指令 （用户）**********

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f));
        auto gltime =  (float)glfwGetTime();
        transform = glm::rotate(transform, gltime*2, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scal, scal, scal));
        if(scal >= 1.0)  // 自己实现的缩放效果
        {
            flag = false;
        }
        else if(scal <= 0.1)
        {
            flag = true;
        }

        if(flag)
        {
            scal +=0.1;
        }
        else
        {
            scal -= 0.1;
        }

        // get matrix's uniform location and set matrix
        shader.enable();
        unsigned int transformLoc = glGetUniformLocation(shader.program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        // create transformations
        glm::mat4 transform2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform2 = glm::translate(transform2, glm::vec3(0.5f, -0.5f, 0.0f));
        transform2 = glm::rotate(transform2, (float)glfwGetTime()*4, glm::vec3(0.0f, 0.0f, 1.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        transform2 = glm::scale(transform2, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        // get matrix's uniform location and set matrix
        shader.enable();
        unsigned int transformLoc2 = glGetUniformLocation(shader.program, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transform2));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        // create transformations
        glm::mat4 transform3 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform3 = glm::translate(transform3, glm::vec3(-0.5f, 0.5f, 0.0f));
        transform3 = glm::rotate(transform3, (float)glfwGetTime()*6, glm::vec3(0.0f, 0.0f, 1.0f));
        transform3 = glm::scale(transform3, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        // get matrix's uniform location and set matrix
        shader.enable();
        unsigned int transformLoc3 = glGetUniformLocation(shader.program, "transform");
        glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(transform3));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // create transformations
        glm::mat4 transform4 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform4 = glm::translate(transform4, glm::vec3(-0.5f, -0.5f, 0.0f));
        transform4 = glm::rotate(transform4, (float)glfwGetTime()*8, glm::vec3(0.0f, 0.0f, 1.0f));
        transform4 = glm::scale(transform4, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        // get matrix's uniform location and set matrix
        shader.enable();
        unsigned int transformLoc4 = glGetUniformLocation(shader.program, "transform");
        glUniformMatrix4fv(transformLoc4, 1, GL_FALSE, glm::value_ptr(transform4));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);



        //***************************************

        // 检查并调用事件，交换缓冲 （默认操作）
        glfwSwapBuffers(window);                                     // 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        glfwPollEvents();                                            // 有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态
    }

    // 解绑 VAO  VBO 着色器程序对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


// 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}
