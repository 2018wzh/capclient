#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

class Logger {
public:
    // ��ȡ��־ʵ��������ģʽ��
    static std::shared_ptr<spdlog::logger>& get_instance();

    // ������־����
    static void set_log_level(spdlog::level::level_enum level);

private:
    // ������־��
    static std::shared_ptr<spdlog::logger> create_logger();
};