#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MyLog.h"
#include "utils/ShaderUtils.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    LOGI("called");
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


int main() {
    // 使用GLFW前需要init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 通过GLAD使用GLAPI前需要init
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader{"../src/VertexShader.glsl", "../src/FragmentShader.glsl"};

    float vertices[] = {
            // 位置              // 颜色
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    unsigned int VBO; // 顶点缓冲对象：Vertex Buffer Object，VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 放置到某种类型的槽位上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作

    shader.use();

    // 不是必须使用VAO，这只是CPU->GPU的一个过程，画之前需要穿一次数据给GPU。这里预先传给GPU方便后续直接使用。
    unsigned int VAO; // 顶点数组对象：Vertex Array Object，VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // 放置到某种类型的槽位上，直接通过函数名指定
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
// 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);


    while (!glfwWindowShouldClose(window)) {
        // 输入
        processInput(window);

        // 线框模式
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//        glViewport(0, 0, 800, 600); // 由回调自动分配
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置状态：清除后的颜色是什么？
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays 会使用glBindVertexArray这个槽位里的东西

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}