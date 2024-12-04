#include "logger.h"

std::shared_ptr<spdlog::logger>& Logger::get_instance() {
    static std::shared_ptr<spdlog::logger> instance = create_logger();
    return instance;
}

std::shared_ptr<spdlog::logger> Logger::create_logger() {
    auto logger = spdlog::stdout_color_mt("capclient");
    logger->set_level(spdlog::level::info); // 默认日志级别为 info
    return logger;
}

void Logger::set_log_level(spdlog::level::level_enum level) {
    get_instance()->set_level(level);
}