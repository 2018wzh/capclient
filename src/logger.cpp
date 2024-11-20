#include "logger.h"
std::shared_ptr<spdlog::logger>& Logger::get_instance() {
    static std::shared_ptr<spdlog::logger> instance = create_logger();
    return instance;
}
std::shared_ptr<spdlog::logger> Logger::create_logger() {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);
        console_sink->set_pattern("%^[%l] %v%$");
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
        file_sink->set_level(spdlog::level::info);
        std::vector<spdlog::sink_ptr> sinks = { console_sink, file_sink };
        auto logger = std::make_shared<spdlog::logger>("multi_sink_logger", sinks.begin(), sinks.end());
        spdlog::set_default_logger(logger);
        return logger;
}