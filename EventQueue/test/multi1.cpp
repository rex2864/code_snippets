#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>
#include "multi1.h"

using namespace Kyle;

Multi1* Multi1::instance = nullptr;

Multi1::Multi1()
{
}

Multi1::~Multi1()
{
}

Multi1* Multi1::GetInstance()
{
    if(Multi1::instance == nullptr)
    {
        Multi1::instance = new Multi1();
    }
    return Multi1::instance;
}

void Multi1::Destroy()
{
    if(Multi1::instance != nullptr)
    {
        delete Multi1::instance;
        Multi1::instance = nullptr;
    }
}

std::string Multi1::GetName()
{
    return "Multi1";
}

void Multi1::SetEQInstance(EventQueue* eq)
{
    this->eq = eq;
}

bool Multi1::Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size)
{
    std::chrono::system_clock::time_point tp_now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dur = tp_now.time_since_epoch();
    uint64_t ts = dur.count();
    std::string time_str = std::to_string(ts / 1000000);
    time_str += ".";
    time_str += std::to_string(ts % 1000000);

    std::cout << "Multi1 -> pidx: " << pidx << " " << time_str << std::endl;
    if(pidx < 10)
    {
        this->eq->AddItem(Multi1::GetName(), pidx + 10, 0, EventQueue::Delay_1s * 5);
    }
    this->Process_NULL();

    return true;
}

void Multi1::Process_NULL()
{
    //std::cout << "Multi1::Process_NULL()" << std::endl;
}
