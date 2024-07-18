#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    // 顶点着色器源码
const char *vertexShderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"  // 位置变量的属性位置值为0
    "out vec4 vertexColor;"                  // 为片段着色器指定一个颜色输出
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos,1.0);\n"     // 把一个vec3作为vec4的构造器的参数
    "   vertexColor = vec4(0.5,0.0,0.2,1.0);\n"  // 把输出变量设置为当前颜色
    "}\0";
    // 片段着色器源码
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4  vertexColor;\n"   // 从顶点着色器传来的输入变量（名称相同、类型相同）
    "void main()\n"
    "{\n"
    "   FragColor = vertexColor;\n"
    "}\0";
//******************************************
int main()
{
    cout << "shaders" << endl;

// GLFW (窗口)
    glfwInit();                                                         // 初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);                       // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);                       // 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);       // 使用核心模式
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Mac OS X系统

    // 创建一个GLFW窗口对象
    GLFWwindow* window = glfwCreateWindow(800,600,"shaders",NULL,NULL);
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

    // 顶点着色器对象
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);           // 创建顶点着色器对象
    glShaderSource(vertexShader,1,&vertexShderSource,NULL);    // 把顶点着色器源码附加到顶点着色器对象上
    glCompileShader(vertexShader);                             // 编译顶点着色器对象
    int success;                                               // 检测顶点着色器对象编译是否成功
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout<<"Error::shader::vertex::compilation_failed\n"<<infoLog<<std::endl;
    }

    // 片段着色器对象
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);         // 创建片段着色器对象
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL); // 把片段着色器源码附加到片段着色器对象上
    glCompileShader(fragmentShader);                             // 编译片段着色器对象
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);    // 检查片段着色器对象是否编译成功
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout<<"Error::shader::vertex::compilation_failed\n"<<infoLog<<std::endl;
    }

    // 着色器程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();                         // 创建着色器程序对象
    glAttachShader(shaderProgram,vertexShader);                // 附加顶点着色器到着色器程序对象上
    glAttachShader(shaderProgram,fragmentShader);              // 附加片段着色器到着色器程序对象上
    glLinkProgram(shaderProgram);                              // 链接着色器程序对象
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);   // 检查着色器程序对象是否链接成功
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    // 把着色器对象链接到程序对象以后，删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 使用着色器程序对象
    glUseProgram(shaderProgram);


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
        glUseProgram(shaderProgram);
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
    glDeleteProgram(shaderProgram);

// 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}
