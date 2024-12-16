#include "logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

std::shared_ptr<spdlog::logger>& Logger::get_instance() {
    static std::shared_ptr<spdlog::logger> instance = create_logger();
    instance->flush();
    return instance;
}

std::shared_ptr<spdlog::logger> Logger::create_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("capclient.log", true);
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};

    auto logger = std::make_shared<spdlog::logger>("capclient", sinks.begin(), sinks.end());
    spdlog::register_logger(logger); // 注册 logger
    logger->set_level(spdlog::level::info); // 默认日志级别为 info
    logger->flush_on(spdlog::level::info); // 自动同步
    return logger;
}

void Logger::set_log_level(spdlog::level::level_enum level) {
    get_instance()->set_level(level);
}