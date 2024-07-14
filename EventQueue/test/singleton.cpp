#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>
#include "singleton.h"

using namespace Kyle;

Singleton* Singleton::instance = nullptr;

Singleton::Singleton()
{
}

Singleton::~Singleton()
{
}

Singleton* Singleton::GetInstance()
{
    if(Singleton::instance == nullptr)
    {
        Singleton::instance = new Singleton();
    }
    return Singleton::instance;
}

void Singleton::Destroy()
{
    if(Singleton::instance != nullptr)
    {
        delete Singleton::instance;
        Singleton::instance = nullptr;
    }
}

std::string Singleton::GetName()
{
    return "Singleton";
}

bool Singleton::Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size)
{
    std::chrono::system_clock::time_point tp_now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dur = tp_now.time_since_epoch();
    uint64_t ts = dur.count();
    std::string time_str = std::to_string(ts / 1000000);
    time_str += ".";
    time_str += std::to_string(ts % 1000000);

    std::cout << "Singleton -> pidx: " << pidx << " " << time_str << std::endl;
    if(pidx < 10)
    {
        EventQueue::GetInstance()->AddItem(Singleton::GetName(), pidx + 10, 0, EventQueue::Delay_1s * 5);
    }
    this->Process_NULL();

    return true;
}

void Singleton::Process_NULL()
{
    //std::cout << "Singleton::Process_NULL()" << std::endl;
}

int32_t main()
{
    std::cout << "Singleton EventQueue test" << std::endl;

    EventQueue* eq = EventQueue::GetInstance();
    eq->Initialize();
    (void)eq->AddBase(Singleton::GetName(), dynamic_cast<EventBase*>(Singleton::GetInstance()));
    (void)eq->AddItem(Singleton::GetName(), 0, 0);
    (void)eq->AddItem(Singleton::GetName(), 1, 0, EventQueue::Delay_1s);
    (void)eq->AddItem(Singleton::GetName(), 2, 0, EventQueue::Delay_1s * 2);
    (void)eq->AddItem(Singleton::GetName(), 3, 0, EventQueue::Delay_1s * 3);
    (void)eq->AddItem(Singleton::GetName(), 4, 0, EventQueue::Delay_1s * 4);
    std::this_thread::sleep_for(std::chrono::seconds(15));
    eq->Finalize();
    Singleton::Destroy();
    EventQueue::Destroy();

    return 0;
}
