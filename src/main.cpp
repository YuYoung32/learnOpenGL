#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MyLog.h"
#include "utils/ShaderUtils.h"
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    LOGI("called");
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


int main()
{
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
    shader.use();

    // VAO用来配置顶点的属性, 本身并不存储顶点
    unsigned int VAO; // 顶点数组对象：Vertex Array Object，VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // 放置到某种类型的槽位上，直接通过函数名指定

    // VBO
    float vertices[] = {
            // positions          // colors           // texture0 coords
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    // 不是必须使用VBO，这只是CPU->GPU的一个过程，画之前需要穿一次数据给GPU。这里预先传给GPU方便后续直接使用。
    unsigned int VBO; // 顶点缓冲对象：Vertex Buffer Object，VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 放置到某种类型的槽位上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作

    // EBO
    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形

            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
    unsigned int EBO; // 元素缓冲对象：Element Buffer Object，EBO 存储 OpenGL 用来决定要绘制哪些顶点的索引 或 索引缓冲对象 Index Buffer Object，IBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 放置到某种类型的槽位上
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture0 coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Texture
    int width, height, nrChannels;

    unsigned char *data = stbi_load("../src/assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        LOGI("Load assets container");
    }
    unsigned int texture0;
    glGenTextures(1, &texture0);
    LOGI("glGenTextures texture0={}", texture0);
    glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture0);
    // 纹理超出后的环绕方式 S T R 类似 X Y Z
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 纹理缩放后的采样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    data = nullptr;

    data = stbi_load("../src/assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        LOGI("Load assets awesomeface");
    }
    unsigned int texture1;
    glGenTextures(1, &texture1);
    LOGI("glGenTextures texture1={}", texture1);
    glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 纹理超出后的环绕方式 S T R 类似 X Y Z
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 纹理缩放后的采样方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    shader.setInt("texture1", 0); // 0代表GL_TEXTURE0
    shader.setInt("texture2", 1); // 1代表GL_TEXTURE0

    while (!glfwWindowShouldClose(window)) {
        // 输入
        processInput(window);

        // 线框模式
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//        glViewport(0, 0, 800, 600); // 由回调自动分配
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置状态：清除后的颜色是什么？
        glClear(GL_COLOR_BUFFER_BIT);

//        glBindTexture(GL_TEXTURE_2D, texture0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                       0); // glDrawElements 会使用glBindVertexArray和GL_ELEMENT_ARRAY_BUFFER 这两个槽位里的东西

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}