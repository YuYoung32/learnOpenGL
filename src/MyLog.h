//
// Created by 29011 on 2025/3/8.
//

#ifndef LEARNOPENGL_MYLOG_H
#define LEARNOPENGL_MYLOG_H

#include <fmt/core.h>
#include <iostream>
#include <chrono>
#include <iomanip>

std::string GetCurrentTime()
{
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 转换为时间戳（自纪元以来的毫秒数）
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    // 获取当前时间
    std::time_t currentTime = seconds; // 将秒数转换为time_t类型
    std::tm *tmPtr = std::localtime(&currentTime);

    // 使用 stringstream 格式化输出
    std::stringstream ss;
    ss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << milliseconds % 1000;

    return ss.str(); // 返回字符串
}

#define LOGI(format, ...) \
    fmt::print("[INFO] [{}:{} {}] " format "\n", __FILE__, __LINE__, GetCurrentTime(), ##__VA_ARGS__)

#define LOGE(format, ...) \
    fmt::print(stderr, "[ERROR] [{}:{} {}] " format "\n", __FILE__, __LINE__,  GetCurrentTime(), ##__VA_ARGS__)
#endif //LEARNOPENGL_MYLOG_H
