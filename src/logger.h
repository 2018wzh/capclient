#pragma once
#include <spdlog/spdlog.h>
#include <string>

class Logger
{
public:
    static std::shared_ptr<spdlog::logger> &get_instance();
    static std::shared_ptr<spdlog::logger> create_logger();
    static void set_log_level(spdlog::level::level_enum level);

private:
    static void cleanup_old_logs(const std::string &log_dir, int days_to_keep = 3);
    static std::string get_temp_log_path();
};