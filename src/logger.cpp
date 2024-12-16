#include "logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <memory>

std::shared_ptr<spdlog::logger> &Logger::get_instance()
{
    static std::shared_ptr<spdlog::logger> instance = create_logger();
    return instance;
}

std::string Logger::get_temp_log_path()
{
    auto temp_dir = std::filesystem::temp_directory_path() / "capclient_logs";
    std::filesystem::create_directories(temp_dir);

    auto now = std::chrono::system_clock::now();
    auto now_t = std::chrono::system_clock::to_time_t(now);
    char timestamp[32];
    std::tm tm;
    localtime_s(&tm, &now_t);
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", &tm);

    return (temp_dir / (std::string("capclient_") + timestamp + ".log")).string();
}

void Logger::cleanup_old_logs(const std::string &log_dir, int days_to_keep)
{
    auto now = std::filesystem::file_time_type::clock::now();

    for (const auto &entry : std::filesystem::directory_iterator(log_dir))
    {
        auto last_write_time = std::filesystem::last_write_time(entry.path());
        auto age = std::chrono::duration_cast<std::chrono::hours>(now - last_write_time).count();

        if (age > days_to_keep * 24)
        {
            std::filesystem::remove(entry.path());
        }
    }
}

std::shared_ptr<spdlog::logger> Logger::create_logger()
{
    std::string log_path = get_temp_log_path();
    auto temp_dir = std::filesystem::path(log_path).parent_path().string();

    cleanup_old_logs(temp_dir);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, true);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("capclient", sinks.begin(), sinks.end());

    logger->set_level(spdlog::level::info);
    return logger;
}

void Logger::set_log_level(spdlog::level::level_enum level)
{
    get_instance()->set_level(level);
}