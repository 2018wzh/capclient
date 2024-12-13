// src/mq.cpp

// mq.cpp
#define NOMINMAX
#include <mqtt/async_client.h>
#include <thread>
#include <atomic>
#include <queue>
#include <mqtt/callback.h>
#include <condition_variable>
#include <mutex>
#include <unordered_map>
#include "config.h"
#include "cmd.h"
#include "mq.h"
#include "utils.h"
#include "logger.h"
#include "utils.h"

namespace MQ
{
    static class callback : public virtual mqtt::callback
    {
    public:
        void message_arrived(mqtt::const_message_ptr msg) override
        {
            auto topic = msg->get_topic();
            Logger::get_instance()->debug("Message received on topic");
            auto payloadJson = Utils::toJson(msg->to_string());
            std::string action = payloadJson["action"].asString();
            g_SessionID = payloadJson["sessionId"].asString();
            cmd::exec(action);
        }
    };

    std::atomic<bool> Running;
    std::thread Thread;
    std::queue<Event::Journal> messageQueue;
    std::mutex queueMutex;
    std::condition_variable cv;

    // ȫ�� MQTT �ͻ���ָ��
    mqtt::async_client *clientPtr = nullptr;
    std::shared_ptr<callback> cbptr;
    void eventLoop()
    {
        while (Running)
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, []
                    { return !messageQueue.empty() || !Running; });
            while (!messageQueue.empty())
            {
                auto msg = messageQueue.front();
                messageQueue.pop();
                lock.unlock();

                // �� journalEvent ת��Ϊ�ַ�������ʵ�� toJson() ������
                std::string payload = Utils::toStr(msg);
                std::string channel = Config::mqName;
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

    void Connect()
    {
        const std::string ADDRESS = Config::mqServer; // MQTT ��������ַ������ "tcp://localhost:1883"
        const std::string CLIENT_ID = "capclient_publisher";

        clientPtr = new mqtt::async_client(ADDRESS, CLIENT_ID);

        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);

        // ����Ƿ������� vhost
        std::string username = Config::mqUser;
        if (!Config::mqVHost.empty())
            username += ":" + Config::mqVHost;
        connOpts.set_user_name(username);
        connOpts.set_password(Config::mqPass);
        cbptr = std::make_shared<callback>();
        clientPtr->set_callback(*cbptr);
        Logger::get_instance()->debug("Connecting to MQTT server: {}", ADDRESS);
        try
        {
            // ���ӵ� MQTT ������
            mqtt::token_ptr conntok = clientPtr->connect(connOpts);
            conntok->wait();
            Running = true;
            clientPtr->subscribe(Config::mqController, 0)->wait();
            Logger::get_instance()->debug("Subscribed to topic: {}", Config::mqController);

            Thread = std::thread(eventLoop);
        }
        catch (std::exception &e)
        {
            Logger::get_instance()->error("MQTT connect error: {}", e.what());
        }
    }

    void Send(Event::Journal e)
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        Logger::get_instance()->debug("Adding message to queue");
        messageQueue.push(e);
        cv.notify_one();
    }

    void Disconnect()
    {
        Running = false;
        cv.notify_one();
        if (Thread.joinable())
            Thread.join();

        try
        {
            if (clientPtr)
            {
                clientPtr->disconnect()->wait();
                delete clientPtr;
                clientPtr = nullptr;
            }
        }
        catch (const mqtt::exception &e)
        {
            Logger::get_instance()->error("MQTT disconnect error: {}", e.what());
        }
    }

    void Send(std::string s)
    {
        std::string channel = Config::mqName;
        try
        {
            mqtt::message_ptr pubmsg = mqtt::make_message(channel, s);
            pubmsg->set_qos(1);
            if (clientPtr && clientPtr->is_connected())
                clientPtr->publish(pubmsg)->wait();
            else
                throw std::exception("MQTT client is not connected.");
            Logger::get_instance()->info("Send message successfully");
        }
        catch (std::exception e)
        {
            Logger::get_instance()->error("Send message error: {}", e.what());
        }
    }
}
