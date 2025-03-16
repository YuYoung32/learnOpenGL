//
// Created by 29011 on 2025/3/11.
//
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MyLog.h"
#include "utils/ShaderUtils.h"

#ifndef LEARNOPENGL_EXAMPLES_H
#define LEARNOPENGL_EXAMPLES_H

// Use EBO
int DrawRectangle()
{
    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };
    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形

            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
    unsigned int VBO; // 顶点缓冲对象：Vertex Buffer Object，VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 放置到某种类型的槽位上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作

    unsigned int EBO; // 元素缓冲对象：Element Buffer Object，EBO 存储 OpenGL 用来决定要绘制哪些顶点的索引 或 索引缓冲对象 Index Buffer Object，IBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 放置到某种类型的槽位上
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 针对某种类型的槽位里面的东西做操作

    // 不是必须使用VAO，这只是CPU->GPU的一个过程，画之前需要穿一次数据给GPU。这里预先传给GPU方便后续直接使用。
    unsigned int VAO; // 顶点数组对象：Vertex Array Object，VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // 放置到某种类型的槽位上，直接通过函数名指定
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0); // 针对某种类型的槽位里面的东西做操作，直接通过函数名指定槽位
    glEnableVertexAttribArray(0); // 针对某种类型的槽位里面的东西做操作，直接通过函数名指定槽位

    // 线框模式
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//        glViewport(0, 0, 800, 600); // 由回调自动分配
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置状态：清除后的颜色是什么？
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays 会使用glBindVertexArray这个槽位里的东西

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   0); // glDrawElements 会使用glBindVertexArray和GL_ELEMENT_ARRAY_BUFFER 这两个槽位里的东西

    return 0;
}

#endif //LEARNOPENGL_EXAMPLES_H
