#pragma once
#include <spdlog/spdlog.h>

class Logger {
public:
    // 获取日志实例（单例模式）
    static std::shared_ptr<spdlog::logger>& get_instance();

    // 设置日志级别
    static void set_log_level(spdlog::level::level_enum level);

private:
    // 创建日志器
    static std::shared_ptr<spdlog::logger> create_logger();
};