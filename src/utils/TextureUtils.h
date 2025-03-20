//
// Created by 29011 on 2025/3/20.
//

#ifndef LEARNOPENGL_TEXTUREUTILS_H
#define LEARNOPENGL_TEXTUREUTILS_H

#include "MyLog.h"
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>

class TextureUtils {
public:
    static unsigned int Load2DTexture(const std::string &filePath)
    {
        unsigned int ID;
        // Texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr) {
            LOGE("load data failed");
            assert(data != nullptr);
        }

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        // 纹理超出后的环绕方式 S T R 类似 X Y Z
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // 纹理缩放后的采样方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            LOGE("Unsupported image format with {}", nrChannels);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        data = nullptr;
        LOGI("Load assets successfully, filePath={}, width={}, height={}, nrChannels={}", filePath, width, height, nrChannels);
        return ID;
    }

};

#endif //LEARNOPENGL_TEXTUREUTILS_H
