//
// Created by 29011 on 2025/3/8.
//

#ifndef LEARNOPENGL_MYLOG_H
#define LEARNOPENGL_MYLOG_H

#include <fmt/core.h>
#include <iostream>

#define LOGI(format, ...) \
    fmt::print("[INFO] [{}:{} {}] " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define LOGE(format, ...) \
    fmt::print(stderr, "[ERROR] [{}:{} {}] " format "\n", __FILE__, __LINE__,  __func__, ##__VA_ARGS__)
#endif //LEARNOPENGL_MYLOG_H
