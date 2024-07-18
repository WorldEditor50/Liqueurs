#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
using namespace std;

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
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

//******************************************
int main()
{
    cout << "你好三角形" << endl;

// GLFW (窗口)
    glfwInit();                                                         // 初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);                       // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);                       // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);       // 使用核心模式
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Mac OS X系统

    // 创建一个GLFW窗口对象
    GLFWwindow* window = glfwCreateWindow(800,600,"triangle",NULL,NULL);
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
    // 顶点数组对象 VAO
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);   // 创建顶点数组对象
    glBindVertexArray(VAO);      // 绑定顶点数组对象
    // 顶点缓冲对象 VBO
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);                                       // 绑定顶点缓冲对象
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);  // 把之前定义的顶点数据复制到缓冲的内存中

    Shader shader;
    shader.load("vertexSourceCode.vert", "fragmentSourceCode.frag");

    // 链接顶点属性 （告诉 gpu 如何解析内存中的顶点数据）
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

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
        shader.enable();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);

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
