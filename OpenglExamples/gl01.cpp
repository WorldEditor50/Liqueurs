#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* 回调函数，每当窗口改变大小，视口大小也跟随改变 */
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

int main()
{
    // 初始化GLFW
    glfwInit();
    // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // 次版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* 使用核心模式 */
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    /* 创建一个GLFW窗口对象 */
    GLFWwindow* window = glfwCreateWindow(800, 600,
                                          "LearnOpenGL",
                                          NULL,NULL);
    if (window == NULL) {
        std::cout<< "Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    /* 窗口的上下文设置为当前线程的主上下文 */
    glfwMakeContextCurrent(window);
    /* 初始化GLAD(初始化OpenGL函数指针) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return -1;
    }
    /* 视口 */
    glViewport(0, 0, 800, 600);
    /* 视口跟随窗口大小改变 */
    glfwSetFramebufferSizeCallback(window,
                                   framebuffer_size_callback);
    /* 渲染循环 */
    while(!glfwWindowShouldClose(window)) {
        /* 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
        它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。*/
        glfwSwapBuffers(window);
        /* 有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态 */
        glfwPollEvents();
    }
    /* 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源 */
    glfwTerminate();
    return 0;
}
