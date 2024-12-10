// src/mq.cpp

// mq.cpp
#define NOMINMAX
#include "mq.h"
#include "utils.h"
#include <mqtt/async_client.h>
#include <thread>
#include <atomic>
#include <queue>
#include "logger.h"
#include "config.h"
#include <condition_variable>
#include <mutex>

namespace MQ {
    std::atomic<bool> Running;
    std::thread Thread;
    std::queue<Event::Journal> messageQueue;
    std::mutex queueMutex;
    std::condition_variable cv;

    // ȫ�� MQTT �ͻ���ָ��
    mqtt::async_client* clientPtr = nullptr;

    void eventLoop() {
        while (Running) {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [] { return !messageQueue.empty() || !Running; });

            while (!messageQueue.empty()) {
                auto msg = messageQueue.front();
                messageQueue.pop();
                lock.unlock();

                // �� journalEvent ת��Ϊ�ַ�������ʵ�� toJson() ������
                std::string payload = Utils::toStr(msg);
                std::string channel = Config::mqVHost + "/" + Config::mqName;
                // ���� MQTT ��Ϣ������
                mqtt::message_ptr pubmsg = mqtt::make_message(channel, payload);
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

    void Connect() {
        const std::string ADDRESS = Config::mqServer; // MQTT ��������ַ������ "tcp://localhost:1883"
        const std::string CLIENT_ID = "capclient_publisher";

        clientPtr = new mqtt::async_client(ADDRESS, CLIENT_ID);

        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);

        // ����Ƿ������� vhost
        std::string username = Config::mqUser;
        connOpts.set_user_name(username);
        connOpts.set_password(Config::mqPass);

        Logger::get_instance()->debug("Connecting to MQTT server: {}", ADDRESS);
        try {
            // ���ӵ� MQTT ������
            mqtt::token_ptr conntok = clientPtr->connect(connOpts);
            conntok->wait();
            Running = true;
            Thread = std::thread(eventLoop);
        }
        catch (std::exception& e) {
            Logger::get_instance()->error("MQTT CONNECT ERROR: {}", e.what());
        }
    }

    void Send(Event::Journal e) {
        std::lock_guard<std::mutex> lock(queueMutex);
        Logger::get_instance()->debug("Adding message to queue");
        messageQueue.push(e);
        cv.notify_one();
    }

    void Disconnect() {
        Running = false;
        cv.notify_one();
        if (Thread.joinable())
            Thread.join();

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

    void Send(std::string s) {
        std::string channel = Config::mqVHost + "/" + Config::mqName;
        mqtt::message_ptr pubmsg = mqtt::make_message(channel, s);
        pubmsg->set_qos(1);
        if (clientPtr && clientPtr->is_connected())
            clientPtr->publish(pubmsg)->wait();
        else
            Logger::get_instance()->error("MQTT client is not connected.");
    }
}
