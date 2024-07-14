#include <thread>
#include "EventQueue.h"

using namespace Kyle;

EventBase::EventBase()
{
}

EventBase::~EventBase()
{
}

//--------------------------------------------------------------------------------

EventQueue* EventQueue::instance = nullptr;

EventQueue::EventQueue()
    : run_loop(false), thread_count(0)
{
}

EventQueue::~EventQueue()
{
}

EventQueue* EventQueue::GetInstance()
{
    if(EventQueue::instance == nullptr)
    {
        EventQueue::instance = new EventQueue();
    }
    return EventQueue::instance;
}

void EventQueue::Destroy()
{
    if(EventQueue::instance != nullptr)
    {
        delete EventQueue::instance;
        EventQueue::instance = nullptr;
    }
}

bool EventQueue::Initialize()
{
    this->run_loop = true;
    this->thread_count = 0;
    std::thread event_loop(EventQueue::EventLoop, this);
    event_loop.detach();
    std::thread event_loop_trigger(EventQueue::EventLoopTrigger, this);
    event_loop_trigger.detach();

    return true;
}

void EventQueue::Finalize()
{
    this->run_loop = false;

    std::unique_lock<std::mutex> lck(this->thread_mutex);
    this->thread_cv.notify_one();
    lck.unlock();

    while(this->thread_count > 0)
    {
        std::this_thread::sleep_for(this->BASE_TICK);
    }

    this->bases.clear();

    this->item_mutex.lock();
    for(std::list<EventItem*>::iterator it = this->items.begin(); it != this->items.end(); ++it)
    {
        if((*it)->data != nullptr)
        {
            delete (*it)->data;
        }
        delete *it;
    }
    this->items.clear();
    this->item_mutex.unlock();
}

bool EventQueue::AddBase(const std::string cid, EventBase* obj)
{
    bool ret = false;

    do
    {
        if((cid.empty() == true) || (this->bases.find(cid) != this->bases.end()) || (obj == nullptr))
        {
            break;
        }

        this->bases.insert(std::pair<std::string,EventBase*>(cid, obj));
        ret = true;
    } while(false);

    return ret;
}

bool EventQueue::AddItem(const std::string cid, const uint32_t pidx, const uint32_t sidx, const uint32_t delay, const char* data, const uint32_t size)
{
    bool ret = false;

    do
    {
        if((cid.empty() == true) || (this->bases.find(cid) == this->bases.end()))
        {
            break;
        }

        EventItem* item = new EventItem();
        item->cid = cid;
        item->pidx = pidx;
        item->sidx = sidx;
        item->delay = std::chrono::milliseconds(delay);
        if((data != nullptr) && (size > 0))
        {
            item->data = new char[size];
            (void)memcpy(item->data, data, size);
            item->size = size;
        }
        item->timepoint = std::chrono::steady_clock::now();

        this->item_mutex.lock();
        this->items.push_back(item);
        this->item_mutex.unlock();

        std::unique_lock<std::mutex> lck(this->thread_mutex);
        this->thread_cv.notify_one();
        lck.unlock();

        ret = true;
    } while(false);

    return ret;
}

void EventQueue::EventLoop(EventQueue* eq)
{
    if(eq == nullptr)
    {
        return;
    }

    eq->thread_count++;

    do
    {
        std::unique_lock<std::mutex> lck(eq->thread_mutex);
        eq->thread_cv.wait(lck);
        lck.unlock();

        std::chrono::steady_clock::time_point timepoint = std::chrono::steady_clock::now();

        std::list<EventItem*>::iterator it = eq->items.begin();
        while(it != eq->items.end())
        {
            if(std::chrono::duration_cast<std::chrono::milliseconds>(timepoint - (*it)->timepoint) >= (*it)->delay)
            {
                EventItem* item = (*it);
                eq->item_mutex.lock();
                it = eq->items.erase(it);
                eq->item_mutex.unlock();

                std::map<std::string,EventBase*>::iterator itbase = eq->bases.find(item->cid);
                if(itbase == eq->bases.end())
                {
                    delete item;
                    item = nullptr;
                    continue;
                }
                EventBase* base = itbase->second;

                (void)base->Process(item->pidx, item->sidx, item->data, item->size);

                if(item->data != nullptr)
                {
                    delete item->data;
                    item->data = nullptr;
                }
                delete item;
                item = nullptr;
            }
            else
            {
                it++;
            }
        }
    } while(eq->run_loop == true);

    eq->thread_count--;
}

void EventQueue::EventLoopTrigger(EventQueue* eq)
{
    if(eq == nullptr)
    {
        return;
    }

    eq->thread_count++;

    do
    {
        std::this_thread::sleep_for(eq->BASE_TICK);

        std::unique_lock<std::mutex> lck(eq->thread_mutex);
        eq->thread_cv.notify_one();
        lck.unlock();
    } while(eq->run_loop == true);

    eq->thread_count--;
}
