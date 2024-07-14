#ifndef _KYLE_EVENTQUEUE_H_
#define _KYLE_EVENTQUEUE_H_

#include <cstdint>
#include <chrono>
#include <condition_variable>
#include <list>
#include <map>
#include <mutex>
#include <string>

namespace Kyle
{
    class EventBase
    {
    public:
        EventBase();
        virtual ~EventBase();
        virtual bool Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size) = 0;
    };

    class EventQueue
    {
    private:
        class EventItem
        {
        public:
            std::string cid;
            uint32_t pidx;
            uint32_t sidx;
            std::chrono::milliseconds delay;
            char* data;
            uint32_t size;
            std::chrono::steady_clock::time_point timepoint;
        };
        const std::chrono::milliseconds BASE_TICK = std::chrono::milliseconds(100);
    public:
        static const uint32_t Delay_100ms = 100;
        static const uint32_t Delay_200ms = Delay_100ms * 2;
        static const uint32_t Delay_300ms = Delay_100ms * 3;
        static const uint32_t Delay_400ms = Delay_100ms * 4;
        static const uint32_t Delay_500ms = Delay_100ms * 5;
        static const uint32_t Delay_1s    = Delay_500ms * 2;

    public:
        EventQueue();
        ~EventQueue();
        static EventQueue* GetInstance();
        static void Destroy();
        bool Initialize();
        void Finalize();
        bool AddBase(const std::string cid, EventBase* obj);
        bool AddItem(const std::string cid, const uint32_t pidx, const uint32_t sidx, const uint32_t delay = 0, const char* data = nullptr, const uint32_t size = 0);
    private:
        static void EventLoop(EventQueue* eq);
        static void EventLoopTrigger(EventQueue* eq);

    private:
        static EventQueue* instance;
        std::map<std::string,EventBase*> bases;
        std::list<EventItem*> items;
        std::mutex item_mutex;
        bool run_loop;
        int32_t thread_count;
        std::mutex thread_mutex;
        std::condition_variable thread_cv;
    };
}

#endif //_KYLE_EVENTQUEUE_H_
