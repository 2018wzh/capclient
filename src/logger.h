#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

class Logger {
public:
    // 获取日志实例（单例模式）
    static std::shared_ptr<spdlog::logger>& get_instance();

private:
    // 创建日志器
    static std::shared_ptr<spdlog::logger> create_logger();
};