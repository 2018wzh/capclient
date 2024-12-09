// mq.cpp
//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "mq.h"
#include <mqtt/async_client.h>
#include <thread>
#include <atomic>
#include <queue>
#include "logger.h"
#include "config.h"
#include <condition_variable>
#include <mutex>

std::atomic<bool> MQ::Running;
std::thread MQ::Thread;
std::queue<Event::Journal> messageQueue;
std::mutex queueMutex;
std::condition_variable cv;

// 移动 mqtt::async_client 到全局，以便在 MQ::Connect 和 MQ::DIsconnect 中使用
mqtt::async_client* clientPtr = nullptr;

void eventLoop() {
    while (MQ::Running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [] { return !messageQueue.empty() || !MQ::Running; });

        while (!messageQueue.empty()) {
            auto msg = messageQueue.front();
            messageQueue.pop();
            lock.unlock();

            // 将 journalEvent 转换为字符串（需实现 toJson() 方法）
            std::string payload = msg.toJson();

            // 创建 MQTT 消息并发布
            mqtt::message_ptr pubmsg = mqtt::make_message("capture", payload);
            pubmsg->set_qos(1);
            if (clientPtr && clientPtr->is_connected()) 
                clientPtr->publish(pubmsg)->wait();
            else 
                Logger::get_instance()->error("MQTT client is not connected.");
            Logger::get_instance()->debug("Message published");
            lock.lock();
        }
    }
}

void MQ::Connect() {
    const std::string ADDRESS = Config::mqServer; // MQTT 服务器地址，例如 "tcp://localhost:1883"
    const std::string CLIENT_ID = "capclient_publisher";

    clientPtr = new mqtt::async_client(ADDRESS, CLIENT_ID);

    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
	connOpts.set_user_name(Config::mqUser);
	connOpts.set_password(Config::mqPass);
	Logger::get_instance()->debug("Connecting to MQTT server: {}", ADDRESS);
    try {
        // 连接到 MQTT 服务器
        mqtt::token_ptr conntok = clientPtr->connect(connOpts);
        conntok->wait();
        MQ::Running = true;
        MQ::Thread = std::thread(eventLoop);
    }
    catch (const mqtt::exception& e) {
        Logger::get_instance()->error("MQTT CONNECT ERROR: {}", e.what());
    }
}

void MQ::Send(Event::Journal e) {
    std::lock_guard<std::mutex> lock(queueMutex);
	Logger::get_instance()->debug("Adding message to queue");
    messageQueue.push(e);
    cv.notify_one();
}

void MQ::Disconnect() {
    MQ::Running = false;
    cv.notify_one();
    if (MQ::Thread.joinable())
        MQ::Thread.join();

    try {
        if (clientPtr) {
            clientPtr->disconnect()->wait();
            delete clientPtr;
            clientPtr = nullptr;
        }
    }
    catch (const mqtt::exception& e) {
        Logger::get_instance()->error("MQTT DISCONNECT ERROR: {}", e.what());
    }
}
void MQ::Send(std::string s) {
    mqtt::message_ptr pubmsg = mqtt::make_message("capture", s);
    pubmsg->set_qos(1);
    if (clientPtr && clientPtr->is_connected())
        clientPtr->publish(pubmsg)->wait();
    else
        Logger::get_instance()->error("MQTT client is not connected.");
}
